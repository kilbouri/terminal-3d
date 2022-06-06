#ifndef TERM_ENGINE
#define TERM_ENGINE

#include <stdbool.h>

// BUFFER TYPES ========================================

typedef struct {
    char r, g, b;
} Color;

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
    double fovRadians, distanceCorrection;
} FrameConstants;

FrameConstants GetFrameConstants(EngineConfig config);

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

// MESH TYPES ==========================================

typedef struct {
    Vector3 v1, v2, v3, normal;
} Triangle;

typedef struct {
    int triangleCount;
    Triangle triangles[];
} Mesh;

#endif // TERM_ENGINE
