// STL
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Config
#include "EngineConfig.h"

// TermEngine
#include "src/TermEngine/Math/Vector.h"
#include "src/TermEngine/Math/Transform.h"
#include "src/TermEngine/Mesh/Mesh.h"
#include "src/TermEngine/Rendering/Drawing.h"
#include "src/TermEngine/Rendering/Buffer.h"
#include "src/TermEngine/Rendering/Projection.h"
#include "src/TermEngine/Rendering/Output.h"
#include "src/TermEngine/TermEngine.h"
#include "src/TermEngine/Debug.h"

// Other
#include "src/TerminalManip.h"

void ExitHandler();
void SigIntHandler(int sigNum);
void Die(int status, const char* err);
FILE* OpenOrDie(const char* path);
long MilliElapsed(time_t now, time_t before);

int main(int argc, char** argv) {
    SetRawInput(true);
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
        .zNear = Z_NEAR,
        .showBackfaces = !BACKFACE_CULLING,
        .wireframeMode = RENDER_WIRE_FRAME,
    };

    // Read model from file (die on fail)
    FILE* modelFile = OpenOrDie(MODEL_SOURCE);
    Mesh* model = LoadMeshFromSTL(modelFile);
    fclose(modelFile);

    if (model == NULL) {
        Die(ENOENT, "Could not load model.");
    }

    // Allocate required buffers
    ColorBuffer* colorBuffer = GetColorBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    DepthBuffer* depthBuffer = GetDepthBuffer(engineConfig.viewportWidth, engineConfig.viewportHeight);
    time_t frameStart, frameEnd;

    // Initial model transform
    Transform modelTransform = {
        .position = INITIAL_POSTION,
        .rotation = FromEuler(INITIAL_ROTATION),
        .scale = INITIAL_SCALE,
    };

    // Bake constants and start render loop
    FrameConstants consts = GetFrameConstants(engineConfig);
    for (int i = 0; i < 10000; i++) { // stand-in for keyboard button to exit
        time(&frameStart);

        FillMesh(model, modelTransform, colorBuffer, depthBuffer, consts);
        DrawPixel(colorBuffer, depthBuffer, (ScreenPoint) {.x = i % engineConfig.viewportWidth, .y = 0}, COLOR_WHITE);

        // draw the frame output
        SetCursorVisible(false);
        CursorToHome();
        Render(colorBuffer);
        fflush(stdout);
        SetCursorVisible(true);

        // reset buffers for next frame
        ClearColorBuffer(colorBuffer);
        ClearDepthBuffer(depthBuffer);

        modelTransform.rotation = MulQuaternion(FromEuler(ROTATION_PER_FRAME), modelTransform.rotation);

        time(&frameEnd);
        const long elapsedMilli = MilliElapsed(frameEnd, frameStart);
        const long remainingInFrameCycle = (1000 / MAX_FPS) - elapsedMilli;

        if (remainingInFrameCycle > 0) {
            usleep(1000 * remainingInFrameCycle);
        }
    }

    free(model);
    FreeColorBuffer(colorBuffer);
    FreeDepthBuffer(depthBuffer);
    ExitHandler();
    return 0;
}

void ExitHandler() {
    SetRawInput(false);
    SetCursorVisible(true);
}

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

long MilliElapsed(time_t now, time_t before) {
    static const double inverse1kCPS = 1000.0 / CLOCKS_PER_SEC;
    return ((double)before - now) * inverse1kCPS;
}
