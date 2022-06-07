#ifndef TERM_ENGINE
#define TERM_ENGINE

#include <stdbool.h>

// VECTOR TYPES ========================================

typedef struct {
    double x, y;
} Vector2;

typedef struct {
    double x, y, z;
} Vector3;

typedef struct {
    int x, y;
    double depth;
} ScreenPoint;

// MISC TYPES ==========================================

typedef struct {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} Transform;

typedef struct {
    unsigned char r, g, b;
} Color;

// BUFFER TYPES ========================================

typedef struct {
    int w, h;
    Color contents[];
} ColorBuffer;

typedef struct {
    int w, h;
    double contents[];
} DepthBuffer;

ColorBuffer* GetColorBuffer(int width, int height);
DepthBuffer* GetDepthBuffer(int width, int height);
void FreeColorBuffer(ColorBuffer* buff);
void FreeDepthBuffer(DepthBuffer* buff);
void ClearColorBuffer(ColorBuffer* buff);
void ClearDepthBuffer(DepthBuffer* buff);

// ENGINE CONFIG TYPES =================================

typedef struct {
    int viewportWidth, viewportHeight;
    double fovRadians;
    double zNear, zFar;
    bool showBackfaces;
} EngineConfig;

typedef struct {
    int viewportWidth, viewportHeight;
    double aspectRatio;
    double zNear, zFar, frustum;
    double fovRadians, distanceScaling;
} FrameConstants;

FrameConstants GetFrameConstants(EngineConfig config);

// MESH TYPES ==========================================

typedef struct {
    Vector3 v1, v2, v3, normal;
} Triangle;

typedef struct {
    int triangleCount;
    Transform transform;
    Triangle triangles[];
} Mesh;

#endif // TERM_ENGINE
