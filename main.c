// STL
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// Config
#include "EngineConfig.h"

// TermEngine
#include "src/TermEngine/Math/Vector.h"
#include "src/TermEngine/Math/Transform.h"
#include "src/TermEngine/Mesh/Mesh.h"
#include "src/TermEngine/Rendering/Drawing.h"
#include "src/TermEngine/Rendering/Buffer.h"
#include "src/TermEngine/Rendering/Projection.h"
#include "src/TermEngine/TermEngine.h"
#include "src/TermEngine/Debug.h"

// Other
#include "src/TerminalManip.h"

void ExitHandler();
void SigIntHandler(int sigNum);
void Die(int status, const char* err);
FILE* OpenOrDie(const char* path);

void DrawGradient(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, EngineConfig config) {
    float invX = 1.0 / config.viewportWidth;
    float invY = 1.0 / config.viewportHeight;

    for (int y = 0; y <= config.viewportHeight; y++) {
        for (int x = 0; x <= config.viewportWidth; x++) {
            int r = (int)(invX * x * 128);
            int b = (int)(invY * y * 128);

            Color c = {0, b, r};
            DrawPixel(colorBuffer, depthBuffer, (ScreenPoint) {x, y, 0}, c);
        }
    }
}

int main(int argc, char** argv) {
    SetRawIO(true);
    atexit(ExitHandler);
    signal(SIGINT, SigIntHandler);

    // get width and height from parameters
    if (argc <= 2) {
        Die(EINVAL, "Missing one or more required arguments");
    }

    int viewWidth = atoi(argv[1]);
    int viewHeight = atoi(argv[2]);

    EngineConfig engineConfig = {
        .fovRadians = DEG2RAD(FOV_DEGREES),
        .showBackfaces = RENDER_WIRE_FRAME,
        .viewportWidth = viewWidth,
        .viewportHeight = viewHeight,
        .effectiveWidth = viewWidth / MONOSPACE_ASPECT_RATIO,
        .effectiveHeight = viewHeight,
        .zFar = Z_FAR,
        .zNear = Z_NEAR};

    // Engine Initialization
    FILE* modelFile = OpenOrDie(MODEL_SOURCE);
    Mesh* model = LoadMeshFromSTL(modelFile);
    fclose(modelFile);

    if (model == NULL) {
        Die(ENOENT, "Could not load model.");
    }

    ColorBuffer* colorBuffer = GetColorBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    DepthBuffer* depthBuffer = GetDepthBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);

    Transform modelTransform = {
        .position = (Vector3) {.x = 0, .y = 0, .z = 4},
        .rotation = FromEuler((Vector3) {.x = 0, .y = 0, .z = 0}),
        .scale = (Vector3) {.x = 1, .y = 1, .z = 1},
    };

    FrameConstants consts = GetFrameConstants(engineConfig);

    for (int i = 0; i < 1000; i++) {
        CursorToHome();
        SetCursorVisible(false);

        DrawMesh(model, modelTransform, colorBuffer, depthBuffer, consts);
        Render(colorBuffer);

        SetCursorVisible(true);

        ClearColorBuffer(colorBuffer);
        ClearDepthBuffer(depthBuffer);

        modelTransform.rotation = MulQuaternion(
            FromEuler((Vector3) {10, 0, 5}),
            modelTransform.rotation);

        usleep(1000);
    }

    free(model);
    FreeColorBuffer(colorBuffer);
    FreeDepthBuffer(depthBuffer);
    ExitHandler();
    return 0;
}

void ExitHandler() { SetRawIO(false); }

void SigIntHandler(int sigNum) {
    ExitHandler();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
}

FILE* OpenOrDie(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL)
        Die(1, "Failed to open model source file.");

    return file;
}

void Die(int status, const char* err) {
    errno = status;
    perror(err);
    exit(status);
}
