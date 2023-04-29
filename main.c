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
#include "EngineConfig.h"

// Libraries
#include "lib/Engine/Engine.h"
#include "lib/Logger/Logger.h"

// Other
#include "src/TerminalManip.h"

bool ShouldExit();
void ExitHandler();
void SigIntHandler(int sigNum);
void Die(int status, const char* err);
unsigned int MilliElapsed(clock_t now, clock_t before);

int main(int argc, char** argv) {
    SetRawInput(true);
    atexit(ExitHandler);
    signal(SIGINT, SigIntHandler);

    // get width and height from parameters
    if (argc <= 2) {
        Die(EINVAL, "Missing one or more required arguments");
    }

    Logger logger = GetLogger("terminal3d.log", LEVEL_INFO_I);

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
    FILE* modelFile = fopen(MODEL_SOURCE, "r");
    Mesh* model = LoadMeshFromSTL(modelFile);

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
    clock_t frameStart, frameEnd;

    // Initial model transform
    Transform modelTransform = {
        .position = INITIAL_POSTION,
        .rotation = FromEuler(INITIAL_ROTATION),
        .scale = INITIAL_SCALE,
    };

    // Bake constants and start render loop
    FrameConstants consts = GetFrameConstants(engineConfig);

    for (int i = 0; !ShouldExit(); i++) {
        frameStart = clock();

        FillMesh(model, modelTransform, colorBuffer, depthBuffer, consts);
        DrawPixel(colorBuffer, depthBuffer, (ScreenPoint) {.x = i % engineConfig.viewportWidth, .y = 0}, COLOR_WHITE);

        SetCursorVisible(false);

        Render(colorBuffer);
        CursorToHome();

        fflush(stdout);
        SetCursorVisible(true);

        modelTransform.rotation = MulQuaternion(modelTransform.rotation, FromEuler(ROTATION_PER_FRAME));

        // reset buffers for next frame
        ClearColorBuffer(colorBuffer);
        ClearDepthBuffer(depthBuffer);

        modelTransform.rotation = MulQuaternion(FromEuler(ROTATION_PER_FRAME), modelTransform.rotation);

        frameEnd = clock();

        unsigned int elapsed = MilliElapsed(frameEnd, frameStart);
        unsigned int targetElapsed = 1000 / MAX_FPS;

        int milliSleep = targetElapsed - elapsed;
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

bool ShouldExit() {
    char charBuf;
    read(STDIN_FILENO, &charBuf, sizeof(char));

    return charBuf == 'q' || charBuf == 'Q';
}

void ExitHandler() {
    SetRawInput(false);
    SetCursorVisibleImm(true);
}

void SigIntHandler(int sigNum) {
    ExitHandler();
    signal(sigNum, SIG_DFL);
    raise(sigNum);
}

void Die(int status, const char* err) {
    errno = status;
    perror(err);
    exit(status);
}

unsigned int MilliElapsed(clock_t now, clock_t before) {
    static const double inverse1kCPS = 1000.0 / CLOCKS_PER_SEC;
    return ((double)now - before) * inverse1kCPS;
}
