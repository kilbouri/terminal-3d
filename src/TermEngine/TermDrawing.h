#ifndef TERM_DRAWING_H
#define TERM_DRAWING_H

#include "TermEngine.h"

void DrawPixel(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color);
void DrawLine(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint from, ScreenPoint to, Color color);
void DrawWireTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color);
void FillTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color);

#endif // TERM_DRAWING_H
