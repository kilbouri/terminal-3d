#ifndef BUFFER_H
#define BUFFER_H

#include "../Engine.h"

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

void Render(ColorBuffer* colorBuffer);
void DoubleBufferedRender(ColorBuffer* primary, ColorBuffer* secondary);

void RenderDepth(DepthBuffer* colorBuffer, float zNear, float zFar);
void DoubleBufferedRenderDepth(DepthBuffer* primary, DepthBuffer* secondary, float zNear, float zFar);

#endif // BUFFER_H
