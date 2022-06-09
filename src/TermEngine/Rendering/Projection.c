#include <stdio.h>

#include "Drawing.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/Transform.h"
#include "../Mesh/Mesh.h"
#include "../TermEngine.h"

int ShouldCullTriangle(Triangle tri);

ScreenPoint Project(Vector3 point, FrameConstants constants) {
    // TODO: have this take a triangle and 3 output ScreenPoints and project
    // an entire triangle in one shot... SIMD will allow parallelization.

    const float f = constants.distanceScaling;
    const float af = constants.effectiveAspectRatio * f;
    const float q = constants.frustum;
    const float negZNearQ = -constants.zNear * q;

    // clang-format off
    Matrix4x4 projectionMatrix = {
        {af, 0,         0, 0},
        { 0, f,         0, 0},
        { 0, 0,         q, 1},
        { 0, 0, negZNearQ, 0},
    }; // clang-format on

    Vector4 projectionCoordinate = {point.x, point.y, point.z, 1};
    Vector4 result = Vector4MulMatrix4x4(projectionCoordinate, projectionMatrix);

    // original Z is now in result.W and needs to divide the other three components (assuming its non-zero)
    float x = result.x;
    float y = result.y;
    float z = result.z;

    if (result.w != 0) {
        float inverseZ = 1.0 / result.w;
        x *= inverseZ;
        y *= inverseZ;
        z *= inverseZ;
    }

    // de-normalize into screen coordinate space
    ScreenPoint screenPoint = {
        .x = (int)(0.5 * (x + 1) * constants.viewportWidth),
        .y = (int)(0.5 * (y + 1) * constants.viewportHeight),
        .depth = z,
    };

    return screenPoint;
}

void DrawMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants) {
    for (int i = 0; i < mesh->triangleCount; i++) {
        Triangle tri = mesh->triangles[i];

        Vector3 v1Transformed = ApplyTransform(tri.v1, meshTransform);
        Vector3 v2Transformed = ApplyTransform(tri.v2, meshTransform);
        Vector3 v3Transformed = ApplyTransform(tri.v3, meshTransform);

        Triangle transformedTriangle = (Triangle) {
            .normal = ZerosVector3,
            .v1 = v1Transformed,
            .v2 = v2Transformed,
            .v3 = v3Transformed,
        };

        if (!constants.backfacesEnabled && ShouldCullTriangle(transformedTriangle)) {
            continue;
        }

        ScreenPoint v1 = Project(v1Transformed, constants);
        ScreenPoint v2 = Project(v2Transformed, constants);
        ScreenPoint v3 = Project(v3Transformed, constants);

        Color c = {80, 255, 80};
        DrawWireTriangle(colorBuffer, depthBuffer, v1, v2, v3, c);
    }
}

int ShouldCullTriangle(Triangle triTransformed) {
    // FIXME: this assumes camera is at Vector3.Zero!

    const Vector3 n1 = SubVector3(triTransformed.v2, triTransformed.v1);
    const Vector3 n2 = SubVector3(triTransformed.v3, triTransformed.v1);

    const Vector3 rawNormal = CrossVector3(n1, n2);
    const Vector3 normal = NormalizeVector3(rawNormal);

    const Vector3 cameraPos = ZerosVector3;
    const Vector3 v1CameraRelative = SubVector3(triTransformed.v1, cameraPos);

    const float dotProduct = DotVector3(normal, v1CameraRelative);
    // printf("V1_Rel = [%f, %f, %f] | Normal = [%f, %f, %f] | DP = %f\n", v1CameraRelative.x, v1CameraRelative.y, v1CameraRelative.z, normal.x, normal.y, normal.z, dotProduct);

    return dotProduct >= 0.0f;
}
