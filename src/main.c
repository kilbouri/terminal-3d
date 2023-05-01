// STL
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Config
#include "Config.h"

// Libraries
#include "../lib/Engine/Engine.h"
#include "../lib/Logger/Logger.h"

// Other
#include "TerminalManip.h"

inline static bool ShouldExit();
inline static void ExitHandler();
inline static void SigIntHandler(int sigNum);
inline static void Die(int status, const char* err);
inline static unsigned int MilliElapsed(clock_t now, clock_t before);

int main(int argc, char** argv) {
    SetRawInput(true);
    atexit(ExitHandler);
    signal(SIGINT, SigIntHandler);

    // get width and height from parameters
    if (argc <= 2) {
        Die(EINVAL, "Missing one or more required arguments");
    }

    Logger logger = GetFileLogger("terminal3d.log", LEVEL_INFO_I);

    int viewWidth = atoi(argv[1]);
    int viewHeight = atoi(argv[2]);

    EngineConfig engineConfig = {
        .fovRadians = DEG2RAD(FOV_DEGREES),
        .viewportWidth = viewWidth,
        .viewportHeight = viewHeight,
        .effectiveWidth = viewWidth / MONOSPACE_ASPECT_RATIO,
        .effectiveHeight = viewHeight,
        .zFar = Z_FAR,
        .zNear = Z_NEAR,
        .backfaceCulling = BACKFACE_CULLING,
        .wireframeMode = RENDER_WIRE_FRAME,
    };

    // Read model from file (die on fail)
    FILE* modelFile = fopen(MODEL_SOURCE, "r");
    Mesh* model = LoadMeshFromSTL(modelFile); // null iff modelFile is null

    if (model == NULL) {
        LogError(logger, "Failed to open " MODEL_SOURCE ". Does the file exist?");
        exit(ENOENT);
    } else {
        LogInfo(logger, "Succesfully opened " MODEL_SOURCE " as STL model");
        fclose(modelFile);
    }

    // Allocate required buffers
    ColorBuffer* colorBuffer = GetColorBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    DepthBuffer* depthBuffer = GetDepthBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    clock_t frameStart, frameEnd, rasterStart, rasterEnd, outputStart, outputEnd;

    // Initial model transform
    Transform modelTransform = {
        .position = INITIAL_POSTION,
        .rotation = FromEuler(INITIAL_ROTATION_DEG),
        .scale = INITIAL_SCALE,
    };

    // Bake constants and start render loop
    FrameConstants frameConstants = GetFrameConstants(engineConfig);

    // Optionally recomite normals
    if (RECOMPUTE_NORMALS) {
        for (int triangleIndex = 0; triangleIndex < model->triangleCount; triangleIndex++) {
            Triangle* triangle = &(model->triangles[triangleIndex]);

            const Vector3 v1 = triangle->v1;
            const Vector3 v2 = triangle->v2;
            const Vector3 v3 = triangle->v3;

            triangle->normal = CrossVector3(SubVector3(v3, v1), SubVector3(v2, v1));
        }
    }

    for (int i = 0; !ShouldExit(); i++) {
        frameStart = clock();

        int trianglesDrawn = 0;
        int trianglesCulled = 0;

        // TODO: stop assuming that we're actually operating at the FPS cap (could be below for example)
        float deltaTime = 1.0f / MAX_FPS;

        rasterStart = clock();
        for (int triangleIndex = 0; triangleIndex < model->triangleCount; triangleIndex++) {
            Vector3 v1 = ApplyTransform(model->triangles[triangleIndex].v1, modelTransform);
            Vector3 v2 = ApplyTransform(model->triangles[triangleIndex].v2, modelTransform);
            Vector3 v3 = ApplyTransform(model->triangles[triangleIndex].v3, modelTransform);

            // Apply only rotation to the normal
            Vector3 normal = Vector3MulQuaternion(model->triangles[triangleIndex].normal, modelTransform.rotation);

            // Backface culling disabled in wireframe mode (cuz it looks odd)
            if (!engineConfig.wireframeMode && engineConfig.backfaceCulling) {
                float dotProduct = DotVector3(normal, SubVector3(v1, ZerosVector3));

                if (dotProduct >= 0) {
                    trianglesCulled += 1;
                    continue;
                }
            }

            EyeSpace v1Eye = ToEyeSpace(v1, IdentityTransform);
            EyeSpace v2Eye = ToEyeSpace(v2, IdentityTransform);
            EyeSpace v3Eye = ToEyeSpace(v3, IdentityTransform);

            HomoClipSpace v1Clip = ToClipSpace(v1Eye, frameConstants);
            HomoClipSpace v2Clip = ToClipSpace(v2Eye, frameConstants);
            HomoClipSpace v3Clip = ToClipSpace(v3Eye, frameConstants);

            DeviceSpace v1Device = ToNormalDeviceSpace(v1Clip);
            DeviceSpace v2Device = ToNormalDeviceSpace(v2Clip);
            DeviceSpace v3Device = ToNormalDeviceSpace(v3Clip);

            if (!(-1 <= v1Device.x && v1Device.x <= 1) && !(-1 <= v1Device.y && v1Device.y <= 1) &&
                !(-1 <= v2Device.x && v2Device.x <= 1) && !(-1 <= v2Device.y && v2Device.y <= 1) &&
                !(-1 <= v3Device.x && v3Device.x <= 1) && !(-1 <= v3Device.y && v3Device.y <= 1)) {
                trianglesCulled += 1;
                continue;
            }

            ViewportSpace v1View = ToViewportSpace(v1Device, frameConstants);
            ViewportSpace v2View = ToViewportSpace(v2Device, frameConstants);
            ViewportSpace v3View = ToViewportSpace(v3Device, frameConstants);

            ScreenPoint v1Point = (ScreenPoint) {.x = v1View.x, .y = v1View.y, .depth = v1View.depth};
            ScreenPoint v2Point = (ScreenPoint) {.x = v2View.x, .y = v2View.y, .depth = v2View.depth};
            ScreenPoint v3Point = (ScreenPoint) {.x = v3View.x, .y = v3View.y, .depth = v3View.depth};

            Color triangleColor;

            if (SHADING_MODE == SM_INDEX) {
                const float minBrightness = INDEX_AMBIENT_LIGHT;
                const float inverseTriangleCount = (1.0f - minBrightness) / model->triangleCount;
                const float brightness = (inverseTriangleCount * triangleIndex) + minBrightness;

                triangleColor = ColorFromHSV(0, 0, brightness);
            } else if (SHADING_MODE == SM_NORMAL) {
                // Negated components cuz positive normals point into screen and aren't visible lol
                triangleColor = ColorFromRGB(-normal.x, -normal.y, -normal.z);
            } else if (SHADING_MODE == SM_PHONG_SHADING) {
                const float phong = Clamp(DotVector3(normal, NormalizeVector3(LIGHT_DIRECTION)), 0, 1);
                const float brightness = (phong * PHONG_STRENGTH) + PHONG_AMBIENT_LIGHT;

                triangleColor = ColorFromHSV(0, 0, brightness);
            }

            // If we're here, the face was not culled and should be drawn
            trianglesDrawn += 1;
            if (engineConfig.wireframeMode) {
                DrawWireTriangle(colorBuffer, depthBuffer, v1Point, v2Point, v3Point, triangleColor);
            } else {
                FillTriangle(colorBuffer, depthBuffer, v1Point, v2Point, v3Point, triangleColor);
            }
        }

        rasterEnd = clock();
        outputStart = clock();

        SetCursorVisible(false);
        Render(colorBuffer);
        CursorToHome();
        SetCursorVisible(true);

        fflush(stdout);
        outputEnd = clock();

        // reset buffers for next frame
        ClearColorBuffer(colorBuffer);
        ClearDepthBuffer(depthBuffer);

        Vector3 eulerRotation = MulVector3(ROTATION_PER_SECOND, deltaTime);
        modelTransform.rotation = MulQuaternion(FromEuler(eulerRotation), modelTransform.rotation);

        frameEnd = clock();

        unsigned int frameTotal = MilliElapsed(frameEnd, frameStart);
        unsigned int targetElapsed = 1000 / MAX_FPS;

        unsigned int rasterTime = MilliElapsed(rasterEnd, rasterStart);
        unsigned int outputTime = MilliElapsed(outputEnd, outputStart);
        int milliSleep = targetElapsed - frameTotal;

        LogInfo(logger, "Resolution: %d x %d", engineConfig.viewportWidth, engineConfig.viewportHeight);
        LogInfo(logger, "Total frame time: %u ms (of %u for FPS cap)", frameTotal, targetElapsed);
        LogInfo(logger, "\tRaster - %u ms", rasterTime);
        LogInfo(logger, "\tOutput - %u ms", outputTime);
        LogInfo(logger, "\tWait   - %d ms", milliSleep);
        LogInfo(logger, "Total triangles: %d", model->triangleCount);
        LogInfo(logger, "\tCulled - %d", trianglesCulled);
        LogInfo(logger, "\tDrawn  - %d", trianglesDrawn);

        // Recompute wait because writing logs takes a non-insignificant time
        frameEnd = clock();
        milliSleep = targetElapsed - MilliElapsed(frameEnd, frameStart);

        if (milliSleep > 0) {
            usleep(1000 * milliSleep);
        }
    }

    LogInfo(logger, "Exit key pressed, exiting.");

    free(model);
    CloseLogger(logger);
    FreeColorBuffer(colorBuffer);
    FreeDepthBuffer(depthBuffer);
    ExitHandler();
    return 0;
}

inline static bool ShouldExit() {
    char charBuf;
    read(STDIN_FILENO, &charBuf, sizeof(char));

    return charBuf == 'q' || charBuf == 'Q';
}

inline static void ExitHandler() {
    SetRawInput(false);
    SetCursorVisibleImm(true);
}

inline static void SigIntHandler(int sigNum) {
    ExitHandler();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
}

inline static void Die(int status, const char* err) {
    errno = status;
    perror(err);
    exit(status);
}

inline static unsigned int MilliElapsed(clock_t now, clock_t before) {
    static const double inverse1kCPS = 1000.0 / CLOCKS_PER_SEC;
    return ((double)now - before) * inverse1kCPS;
}
