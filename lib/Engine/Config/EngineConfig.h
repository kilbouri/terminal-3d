#include <stdbool.h>

typedef struct {
    int viewportWidth, viewportHeight;
    float effectiveWidth, effectiveHeight;
    float fovRadians;
    float zNear, zFar;
    bool backfaceCulling, wireframeMode;
} EngineConfig;

typedef struct {
    int viewportWidth, viewportHeight;
    float aspectRatio;
    float effectiveAspectRatio;
    float zNear, zFar, frustum;
    float fovRadians, distanceScaling;
    bool backfaceCulling, fillTriangles;
} FrameConstants;

FrameConstants GetFrameConstants(EngineConfig config);
