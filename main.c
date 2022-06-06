#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "./src/TermEngine/TermRendering.h"
#include "./src/TermEngine/TermDrawing.h"
#include "./src/TerminalManip.h"
#include "./src/TermEngine/MeshLoaders/STLMesh.h"
#include "./EngineConfig.h"

void ExitHandler();
void SigIntHandler(int sigNum);
FILE* OpenOrDie(const char* path);

int main(void) {
    SetRawIO(true);
    atexit(ExitHandler);
    signal(SIGINT, SigIntHandler);

    FILE* modelFile = OpenOrDie(MODEL_SOURCE);
    EngineConfig engineConfig = {
        .fovRadians = 0.0174532925 * FOV_DEGREES, // FOV_DEGREES * pi/180
        .showBackfaces = RENDER_WIRE_FRAME,
        .viewportWidth = VIEW_WIDTH,
        .viewportHeight = VIEW_HEIGHT,
        .zFar = Z_FAR,
        .zNear = Z_NEAR
    };

    // Engine Initialization
    Mesh* model = LoadMesh(modelFile);
    fclose(modelFile);

    ColorBuffer* colorBuffer = GetColorBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    DepthBuffer* depthBuffer = GetDepthBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);

    double invX = 1.0 / VIEW_WIDTH;
    double invY = 1.0 / VIEW_HEIGHT;

    for (int y = 0; y <= VIEW_HEIGHT; y++) {
        for (int x = 0; x <= VIEW_WIDTH; x++) {
            int r = (int) (invX * x * 128);
            int b = (int) (invY * y * 128);

            Color c = { 0, b, r };
            DrawPixel(colorBuffer, depthBuffer, (ScreenPoint) { x, y, 0 }, c);
        }
    }

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
    // check with Connor if we use threads
    ExitHandler();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
}

FILE* OpenOrDie(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Failed to open model source file.");
        exit(1);
    }

    return file;
}
