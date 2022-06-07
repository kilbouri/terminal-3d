#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <errno.h>

#include "./src/TermEngine/TermRendering.h"
#include "./src/TermEngine/TermDrawing.h"
#include "./src/TermEngine/TermMath.h"
#include "./src/TermEngine/TermDebug.h"
#include "./src/TerminalManip.h"
#include "./src/TermEngine/MeshLoaders/STLMesh.h"
#include "./EngineConfig.h"

void ExitHandler();
void SigIntHandler(int sigNum);
void Die(int status, const char* err);
FILE* OpenOrDie(const char* path);

void DrawGradient(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, EngineConfig config) {
    double invX = 1.0 / config.viewportWidth;
    double invY = 1.0 / config.viewportHeight;

    for (int y = 0; y <= config.viewportHeight; y++) {
        for (int x = 0; x <= config.viewportWidth; x++) {
            int r = (int) (invX * x * 128);
            int b = (int) (invY * y * 128);

            Color c = { 0, b, r };
            DrawPixel(colorBuffer, depthBuffer, (ScreenPoint) { x, y, 0 }, c);
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
        .fovRadians = 0.0174532925 * FOV_DEGREES, // FOV_DEGREES * pi/180
        .showBackfaces = RENDER_WIRE_FRAME,
        .viewportWidth = viewWidth,
        .viewportHeight = viewHeight,
        .zFar = Z_FAR,
        .zNear = Z_NEAR
    };

    // Engine Initialization
    FILE* modelFile = OpenOrDie(MODEL_SOURCE);
    Mesh* model = LoadMesh(modelFile);
    model->transform = (Transform) {
        .position = (Vector3) {.x = 0, .y = 0, .z = 4},
        .rotation = (Vector3) {.x = 0, .y = 0, .z = 0},
        .scale = (Vector3) {.x = 1, .y = 1, .z = 1}
    };

    fclose(modelFile);

    if (model == NULL) Die(ENOENT, "Could not load model.");

    DumpMesh(model);

    ColorBuffer* colorBuffer = GetColorBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    DepthBuffer* depthBuffer = GetDepthBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);

    DrawMesh(model, colorBuffer, depthBuffer, GetFrameConstants(engineConfig));
    Render(colorBuffer);

    free(model);
    FreeColorBuffer(colorBuffer);
    FreeDepthBuffer(depthBuffer);
    ExitHandler();
    return 0;
}

void ExitHandler() {
    SetRawIO(false);
}

void SigIntHandler(int sigNum) {
    ExitHandler();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
}

FILE* OpenOrDie(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) Die(1, "Failed to open model source file.");

    return file;
}

void Die(int status, const char* err) {
    errno = status;
    perror(err);
    exit(status);
}
