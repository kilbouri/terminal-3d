#ifndef DRAWING_H
#define DRAWING_H

#include "../Math/Utilities.h"
#include "Buffer.h"
#include "Color.h"

typedef struct {
    int x, y;
    float depth;
} ScreenPoint;

void DrawPixel(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color);
void DrawLine(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint from, ScreenPoint to, Color color);
void DrawWireTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color);
void FillTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color);

#endif // DRAWING_H
