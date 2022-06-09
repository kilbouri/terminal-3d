#ifndef PROJECTION_H
#define PROJECTION_H

#include "Buffer.h"
#include "Drawing.h"
#include "../Math/Transform.h"
#include "../Mesh/Mesh.h"
#include "../TermEngine.h"

void Render(ColorBuffer* colorBuffer);
void DrawMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);

#endif // PROJECTION_H
