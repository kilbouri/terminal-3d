#include <stdio.h>

#include "TermEngine.h"
#include "TermRendering.h"

void Render(ColorBuffer* colorBuffer) {
    int numElements = colorBuffer->w * colorBuffer->h;
    for (int i = 0; i < numElements; i++) {
        DrawColor(colorBuffer->contents[i]);
    }
}

void DrawColor(Color c) {
    // Sets background color in RGB, then prints a space.
    printf("\033[48;2;%u;%u;%um ", c.r, c.g, c.b);
}

ScreenPoint Project(Vector3 point, FrameConstants constants) {
    double x = point.x;
    double y = point.y;
    double z = point.z;
    double zInverse = 1 / (z == 0 ? __FLT_EPSILON__ : z);

    // TODO: this can be enhanced with SIMD... especially if we rework
    // to do all three vertices at the same time...
    x = x * constants.aspectRatio * constants.distanceCorrection * zInverse;
    y = y * constants.distanceCorrection * zInverse;
    z = z * constants.frustum - (constants.frustum * constants.zNear);

    ScreenPoint screenPoint = {
        .x = (int) x,
        .y = (int) y,
        .depth = z
    };

    return screenPoint;
}

void DrawMesh(Mesh* mesh, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants) {
    // foreach triangle in mesh
    //      transform vertices
    //      calculate normal
    //      draw transformed

    for (int i = 0; i < mesh->triangleCount; i++) {
        ScreenPoint v1 = Project(mesh->triangles[i].v1, constants);
        ScreenPoint v2 = Project(mesh->triangles[i].v2, constants);
        ScreenPoint v3 = Project(mesh->triangles[i].v3, constants);

        //DrawLine(v1, v2, colorBuffer, depthBuffer);
        //DrawLine(v2, v3, colorBuffer, depthBuffer);
        //DrawLine(v3, v1, colorBuffer, depthBuffer);
    }
}
