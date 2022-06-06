#ifndef TERM_RENDERING_H
#define TERM_RENDERING_H

#include "TermEngine.h"

void Render(ColorBuffer* colorBuffer);
void DrawColor(Color c);
void DrawMesh(Mesh* mesh, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants);

#endif // TERM_RENDERING_H
