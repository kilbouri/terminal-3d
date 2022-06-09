#ifndef TERM_ENGINE
#define TERM_ENGINE

#include <stdbool.h>

// ENGINE CONFIG TYPES =================================

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

#endif // TERM_ENGINE
