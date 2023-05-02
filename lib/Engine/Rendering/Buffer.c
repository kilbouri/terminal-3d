#include <math.h>
#include <stdlib.h>

#include <assert.h>
#include "../Engine.h"

inline static void* AllocBuffer(size_t structSize, int numElements, size_t elementSize) {
    return malloc(structSize + (numElements * elementSize));
}

inline static void MoveCursorTo(int x, int y) {
    printf("\033[%d;%dH", y, x);
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

void RenderColor(ColorBuffer* colorBuffer) {
    int numElements = colorBuffer->w * colorBuffer->h;
    for (int i = 0; i < numElements; i++) {
        WriteColor(colorBuffer->contents[i]);
    }
}

void RenderColorDifference(ColorBuffer* primary, ColorBuffer* secondary) {
    assert(primary->w == secondary->w);
    assert(primary->h == secondary->h);

    for (int y = 0; y < primary->h; y++) {
        for (int x = 0; x < primary->w; x++) {
            const int index = (y * primary->w) + x;

            Color primaryColor = primary->contents[index];
            Color secondaryColor = secondary->contents[index];

            // Skip matching pixels
            if (ColorEquals(primaryColor, secondaryColor)) {
                continue;
            }

            MoveCursorTo(x + 1, y + 1);
            WriteColor(primaryColor);
            fflush(stdout);
        }
    }
}

void RenderDepth(DepthBuffer* depthBuffer, float zNear, float zFar) {
    int numElements = depthBuffer->w * depthBuffer->h;
    float denom = 1.0f / (zFar - zNear);

    for (int i = 0; i < numElements; i++) {
        float rawDepth01 = (depthBuffer->contents[i] - zNear) * denom;
        float clampedDepth01 = Clamp(rawDepth01, 0, 1);
        float brightness = clampedDepth01 * COLOR_CHANNEL_MAX;

        Color color = ColorFromHSV(0, 0, brightness);
        WriteColor(color);
    }
}

void RenderDepthDifference(DepthBuffer* primary, DepthBuffer* secondary, float zNear, float zFar) {
    assert(primary->w == secondary->w);
    assert(primary->h == secondary->h);

    float denom = 1.0f / (zFar - zNear);
    for (int y = 0; y < primary->h; y++) {
        for (int x = 0; x < primary->w; x++) {
            const int index = (y * primary->w) + x;

            float primaryDepth = primary->contents[index];
            float secondaryDepth = secondary->contents[index];

            // Skip matching pixels
            if (!Approx(primaryDepth, secondaryDepth)) {
                continue;
            }

            MoveCursorTo(x + 1, y + 1);

            float rawDepth01 = (primaryDepth - zNear) * denom;
            float clampedDepth01 = Clamp(rawDepth01, 0, 1);
            float brightness = clampedDepth01 * COLOR_CHANNEL_MAX;

            Color color = ColorFromHSV(0, 0, brightness);
            WriteColor(color);
        }
    }
}

void SwapBuffers(void** primary, void** secondary) {
    void* temp = *primary;
    *primary = *secondary;
    *secondary = temp;
}
