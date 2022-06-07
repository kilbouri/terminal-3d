#include <stdio.h>

#include "TermEngine.h"
#include "TermRendering.h"
#include "TermDrawing.h"
#include "TermMath.h"

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
    double zInverse = 1.0 / (z == 0 ? 1 : z);

    // TODO: this can be enhanced with SIMD... especially if we rework
    // to do all three vertices at the same time...
    x = x * constants.aspectRatio * constants.distanceScaling * zInverse;
    y = y * constants.distanceScaling * zInverse;
    z = ((z * constants.frustum) - (constants.frustum * constants.zNear)) * zInverse;

    ScreenPoint screenPoint = {
        .x = (int) (0.5 * (x + 1) * constants.viewportWidth),
        .y = (int) (0.5 * (y + 1) * constants.viewportHeight),
        .depth = z
    };

    return screenPoint;
}

void DrawMesh(Mesh* mesh, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants) {
    Transform meshTransform = mesh->transform;

    for (int i = 0; i < mesh->triangleCount; i++) {
        Triangle tri = mesh->triangles[i];

        Vector3 v1Transformed = Vector3Add(Vector3ScalarMul(tri.v1, meshTransform.scale.x), meshTransform.position);
        Vector3 v2Transformed = Vector3Add(Vector3ScalarMul(tri.v2, meshTransform.scale.y), meshTransform.position);
        Vector3 v3Transformed = Vector3Add(Vector3ScalarMul(tri.v3, meshTransform.scale.z), meshTransform.position);

        ScreenPoint v1 = Project(v1Transformed, constants);
        ScreenPoint v2 = Project(v2Transformed, constants);
        ScreenPoint v3 = Project(v3Transformed, constants);

        Color c = { 0, 130, 130 };
        DrawWireTriangle(colorBuffer, depthBuffer, v1, v2, v3, c);
    }
}
