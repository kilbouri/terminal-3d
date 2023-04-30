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
        .rotation = FromEuler(INITIAL_ROTATION),
        .scale = INITIAL_SCALE,
    };

    // Bake constants and start render loop
    FrameConstants frameConstants = GetFrameConstants(engineConfig);

    for (int i = 0; !ShouldExit(); i++) {
        frameStart = clock();

        int trianglesDrawn = 0;
        int trianglesCulled = 0;

        const float minBrightness = 0.25;
        const float inverseTriangleCount = (1.0f - minBrightness) / model->triangleCount;

        rasterStart = clock();
        for (int triangleIndex = 0; triangleIndex < model->triangleCount; triangleIndex++) {
            Vector3 v1 = ApplyTransform(model->triangles[triangleIndex].v1, modelTransform);
            Vector3 v2 = ApplyTransform(model->triangles[triangleIndex].v2, modelTransform);
            Vector3 v3 = ApplyTransform(model->triangles[triangleIndex].v3, modelTransform);

            EyeSpace v1Eye = ToEyeSpace(v1, IdentityTransform);
            EyeSpace v2Eye = ToEyeSpace(v2, IdentityTransform);
            EyeSpace v3Eye = ToEyeSpace(v3, IdentityTransform);

            // Backface culling disabled in wireframe mode (cuz it looks odd)
            if (!engineConfig.wireframeMode && engineConfig.backfaceCulling) {
                // We don't normalize here because we only care about sign of dotproduct, not magnitude.
                Vector3 normal = CrossVector3(SubVector3(v3Eye, v1Eye), SubVector3(v2Eye, v1Eye));
                float dotProduct = DotVector3(normal, v1Eye);

                if (dotProduct <= 0) {
                    trianglesCulled += 1;
                    continue;
                }
            }

            HomoClipSpace v1Clip = ToClipSpace(v1Eye, frameConstants);
            HomoClipSpace v2Clip = ToClipSpace(v2Eye, frameConstants);
            HomoClipSpace v3Clip = ToClipSpace(v3Eye, frameConstants);

            DeviceSpace v1Device = ToNormalDeviceSpace(v1Clip);
            DeviceSpace v2Device = ToNormalDeviceSpace(v2Clip);
            DeviceSpace v3Device = ToNormalDeviceSpace(v3Clip);

            ViewportSpace v1View = ToViewportSpace(v1Device, frameConstants);
            ViewportSpace v2View = ToViewportSpace(v2Device, frameConstants);
            ViewportSpace v3View = ToViewportSpace(v3Device, frameConstants);

            ScreenPoint v1Point = (ScreenPoint) {.x = v1View.x, .y = v1View.y, .depth = v1View.depth};
            ScreenPoint v2Point = (ScreenPoint) {.x = v2View.x, .y = v2View.y, .depth = v2View.depth};
            ScreenPoint v3Point = (ScreenPoint) {.x = v3View.x, .y = v3View.y, .depth = v3View.depth};

            float brightness = ((inverseTriangleCount * triangleIndex) + minBrightness) * COLOR_CHANNEL_MAX;
            Color triangleColor = (Color) {
                .r = brightness,
                .g = brightness,
                .b = brightness,
            };

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

        modelTransform.rotation = MulQuaternion(FromEuler(ROTATION_PER_FRAME), modelTransform.rotation);

        frameEnd = clock();

        unsigned int frameTotal = MilliElapsed(frameEnd, frameStart);
        unsigned int targetElapsed = 1000 / MAX_FPS;

        unsigned int rasterTime = MilliElapsed(rasterEnd, rasterStart);
        unsigned int outputTime = MilliElapsed(outputEnd, outputStart);
        int milliSleep = targetElapsed - frameTotal;

        LogInfo(logger, "Total frame time: %u ms (of %u for FPS cap)", frameTotal, targetElapsed);
        LogInfo(logger, "\tRaster - %u ms", rasterTime);
        LogInfo(logger, "\tOutput - %u ms", outputTime);
        LogInfo(logger, "\tWait   - %d ms", milliSleep);
        LogInfo(logger, "Total triangles: %d", model->triangleCount);
        LogInfo(logger, "\tCulled - %d", trianglesCulled);
        LogInfo(logger, "\tDrawn  - %d", trianglesDrawn);

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
