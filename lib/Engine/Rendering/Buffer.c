#include <math.h>
#include <stdlib.h>

#include "../Engine.h"

inline static void* AllocBuffer(size_t structSize, int numElements, size_t elementSize) {
    return malloc(structSize + (numElements * elementSize));
}

ColorBuffer* GetColorBuffer(int width, int height) {
    ColorBuffer* buf = AllocBuffer(sizeof(ColorBuffer), width * height, sizeof(Color));
    if (buf == NULL) {
        return NULL;
    }

    buf->w = width;
    buf->h = height;

    ClearColorBuffer(buf);
    return buf;
}

DepthBuffer* GetDepthBuffer(int width, int height) {
    DepthBuffer* buf = AllocBuffer(sizeof(DepthBuffer), width * height, sizeof(float));
    if (buf == NULL) {
        return NULL;
    }

    buf->w = width;
    buf->h = height;

    ClearDepthBuffer(buf);
    return buf;
}

void ClearColorBuffer(ColorBuffer* buff) {
    int size = buff->w * buff->h;
    for (int i = 0; i < size; i++) {
        buff->contents[i] = COLOR_BLACK;
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

void Render(ColorBuffer* colorBuffer) {
    int numElements = colorBuffer->w * colorBuffer->h;
    for (int i = 0; i < numElements; i++) {
        WriteColor(colorBuffer->contents[i]);
    }
}

void RenderDepth(DepthBuffer* depthBuffer, float zNear, float zFar) {
    int numElements = depthBuffer->w * depthBuffer->h;
    float denom = 1.0f / (zFar - zNear);

    for (int i = 0; i < numElements; i++) {
        float rawDepth01 = (depthBuffer->contents[i] - zNear) * denom;
        float clampedDepth01 = Clamp(rawDepth01, 0, 1);
        float brightness = clampedDepth01 * COLOR_CHANNEL_MAX;

        Color color = (Color) {
            .r = brightness,
            .g = brightness,
            .b = brightness,
        };

        WriteColor(color);
    }
}
