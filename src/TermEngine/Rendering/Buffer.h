#ifndef BUFFER_H
#define BUFFER_H

#include "Color.h"

typedef struct {
    int w, h;
    Color contents[];
} ColorBuffer;

typedef struct {
    int w, h;
    float contents[];
} DepthBuffer;

DepthBuffer* GetDepthBuffer(int width, int height);
void ClearDepthBuffer(DepthBuffer* buff);
void FreeDepthBuffer(DepthBuffer* buff);

ColorBuffer* GetColorBuffer(int width, int height);
void ClearColorBuffer(ColorBuffer* buff);
void FreeColorBuffer(ColorBuffer* buff);

#endif // BUFFER_H
