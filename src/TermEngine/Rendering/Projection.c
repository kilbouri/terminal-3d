#include <stdio.h>

#include "Drawing.h"
#include "Pipeline.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"
#include "../Math/Transform.h"
#include "../Mesh/Mesh.h"
#include "../TermEngine.h"

int ShouldCullTriangle(Triangle tri, float* outDotProduct);

ScreenPoint Project(Vector3 point, Transform cameraTransform, FrameConstants constants, bool* discard) {
    // TODO: have this take a triangle and 3 output ScreenPoints and project
    // an entire triangle in one shot... SIMD will allow parallelization.

    *discard = false;

    EyeSpace eyeSpace = ToEyeSpace(point, cameraTransform);
    HomoClipSpace clipSpace = ToClipSpace(eyeSpace, constants);
    DeviceSpace deviceSpace = ToNormalDeviceSpace(clipSpace);

    bool validX = -1 <= deviceSpace.x && deviceSpace.x <= 1;
    bool validY = -1 <= deviceSpace.y && deviceSpace.y <= 1;
    bool validZ = -1 <= deviceSpace.z && deviceSpace.z <= 1;
    *discard = !validX || !validY || !validZ;

    ViewportSpace viewSpace = ToViewportSpace(deviceSpace, constants);
    return viewSpace;
}

void DrawInternal(
    Mesh* mesh, Transform meshTransform,
    ColorBuffer* colorBuffer, DepthBuffer* depthBuffer,
    FrameConstants constants,
    void (*drawCallback)(ColorBuffer*, DepthBuffer*, ScreenPoint, ScreenPoint, ScreenPoint, Color)) {

    const static Transform camera = {
        .position = ZerosVector3,
        .rotation = IdentityQuaternion,
        .scale = OnesVector3,
    };

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

        float dotProduct;
        if (!constants.backfacesEnabled && ShouldCullTriangle(transformedTriangle, &dotProduct)) {
            continue;
        }

        bool shouldDiscard1, shouldDiscard2, shouldDiscard3;
        ScreenPoint v1 = Project(v1Transformed, camera, constants, &shouldDiscard1);
        ScreenPoint v2 = Project(v2Transformed, camera, constants, &shouldDiscard2);
        ScreenPoint v3 = Project(v3Transformed, camera, constants, &shouldDiscard3);

        if (shouldDiscard1 && shouldDiscard2 && shouldDiscard3) {
            // Ignore triangle, its not visible
            continue;
        }

        int brightness = Clamp(255 * Abs(dotProduct) + 0.1f, 0, 255);

        Color c = {.r = brightness, .g = brightness, .b = brightness};
        (*drawCallback)(colorBuffer, depthBuffer, v1, v2, v3, c);
    }
}

void DrawMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants) {
    DrawInternal(mesh, meshTransform, colorBuffer, depthBuffer, constants, DrawWireTriangle);
}

void FillMesh(Mesh* mesh, Transform meshTransform, ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, FrameConstants constants) {
    DrawInternal(mesh, meshTransform, colorBuffer, depthBuffer, constants, FillTriangle);
}

int ShouldCullTriangle(Triangle triTransformed, float* outDotProduct) {
    // FIXME: this assumes camera is at Vector3.Zero!

    const Vector3 n1 = SubVector3(triTransformed.v2, triTransformed.v1);
    const Vector3 n2 = SubVector3(triTransformed.v3, triTransformed.v1);

    const Vector3 rawNormal = CrossVector3(n1, n2);
    const Vector3 normal = NormalizeVector3(rawNormal);

    const Vector3 cameraPos = ZerosVector3;
    const Vector3 v1CameraRelative = SubVector3(triTransformed.v1, cameraPos);

    const double dotProduct = DotVector3(normal, NormalizeVector3(v1CameraRelative));

    if (outDotProduct != NULL) {
        *outDotProduct = dotProduct;
    }

    return dotProduct >= 0.0f;
}
