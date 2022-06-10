#ifndef PROJECTION_H
#define PROJECTION_H

#include "Buffer.h"
#include "Drawing.h"
#include "../Math/Transform.h"
#include "../Mesh/Mesh.h"
#include "../TermEngine.h"

void DrawMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);
void FillMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* ColorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);

#endif // PROJECTION_H
