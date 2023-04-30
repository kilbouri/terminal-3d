#ifndef OUTPUT_H
#define OUTPUT_H

#include "../Engine.h"

void Render(ColorBuffer* colorBuffer);
void RenderDepth(DepthBuffer* colorBuffer, float zNear, float zFar);

#endif // OUTPUT_h
