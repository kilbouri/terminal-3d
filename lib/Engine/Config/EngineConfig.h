#include <stdbool.h>

typedef struct {
    int viewportWidth, viewportHeight;
    float effectiveWidth, effectiveHeight;
    float fovRadians;
    float zNear, zFar;
    bool showBackfaces, wireframeMode;
} EngineConfig;

typedef struct {
    int viewportWidth, viewportHeight;
    float aspectRatio;
    float effectiveAspectRatio;
    float zNear, zFar, frustum;
    float fovRadians, distanceScaling;
    bool backfacesEnabled, fillTriangles;
} FrameConstants;

FrameConstants GetFrameConstants(EngineConfig config);
