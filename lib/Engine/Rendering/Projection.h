#ifndef PROJECTION_H
#define PROJECTION_H

#include "../Engine.h"

void DrawMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);
void FillMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* ColorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);

#endif // PROJECTION_H
