#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TermEngine.h"

void* AllocBuffer(size_t structSize, int numElements, size_t elementSize) {
    return malloc(structSize + (numElements * elementSize));
}

ColorBuffer* GetColorBuffer(int width, int height) {
    ColorBuffer* buf = AllocBuffer(sizeof(ColorBuffer), width * height, sizeof(Color));
    if (buf == NULL) { return NULL; }

    buf->w = width;
    buf->h = height;

    ClearColorBuffer(buf);
    return buf;
}

DepthBuffer* GetDepthBuffer(int width, int height) {
    DepthBuffer* buf = AllocBuffer(sizeof(DepthBuffer), width * height, sizeof(double));
    if (buf == NULL) { return NULL; }

    buf->w = width;
    buf->h = height;

    ClearDepthBuffer(buf);
    return buf;
}

void ClearColorBuffer(ColorBuffer* buff) {
    int size = buff->w * buff->h;
    for (int i = 0; i < size; i++) {
        buff->contents[i] = (Color) { 0,0,0 };
    }
}

void ClearDepthBuffer(DepthBuffer* buff) {
    int size = buff->w * buff->h;
    for (int i = 0; i < size; i++) {
        buff->contents[i] = INFINITY;
    }
}

void FreeColorBuffer(ColorBuffer* buff) {
    free(buff);
}
void FreeDepthBuffer(DepthBuffer* buff) {
    free(buff);
}

FrameConstants GetFrameConstants(EngineConfig config) {
    double aspect = config.viewportWidth / config.viewportHeight;
    double distanceCorrection = 1 / tan(config.fovRadians);
    double frustum = config.zFar / (config.zFar - config.zNear);

    FrameConstants constants = {
        .viewportWidth = config.viewportWidth,
        .viewportHeight = config.viewportHeight,
        .aspectRatio = aspect,
        .zNear = config.zNear,
        .zFar = config.zFar,
        .frustum = frustum,
        .fovRadians = config.fovRadians,
        .distanceCorrection = distanceCorrection
    };

    return constants;
}
