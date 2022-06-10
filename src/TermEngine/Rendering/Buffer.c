#include <math.h>
#include <stdlib.h>

#include "Buffer.h"
#include "Color.h"

void* AllocBuffer(size_t structSize, int numElements, size_t elementSize);

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

void ClearColorBuffer(ColorBuffer* buff) {
    int size = buff->w * buff->h;
    for (int i = 0; i < size; i++) {
        buff->contents[i] = COLOR_BLACK;
    }
}

void FreeColorBuffer(ColorBuffer* buff) {
    free(buff);
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

void ClearDepthBuffer(DepthBuffer* buff) {
    int size = buff->w * buff->h;
    for (int i = 0; i < size; i++) {
        buff->contents[i] = INFINITY;
    }
}

void FreeDepthBuffer(DepthBuffer* buff) {
    free(buff);
}

void* AllocBuffer(size_t structSize, int numElements, size_t elementSize) {
    return malloc(structSize + (numElements * elementSize));
}
