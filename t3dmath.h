#ifndef t3dmath
#define t3dmath

#include <stdio.h>
#include <math.h>

#define cot(r) (1/tan(r))
#define DEG2RAD(d) ((d) * 0.0174532925) // d * pi/180
typedef struct {
  int x, y;
} Int2;

typedef struct {
  float x, y;
} Float2;

typedef struct {
  float x, y, z;
} Float3;

typedef struct {
  Float3 v1, v2, v3, normal;
} Triangle;

typedef struct {
  int triangleCount;
  Triangle* triangles;
} Mesh;

typedef struct {
  int viewportWidth, viewportHeight; // viewport info
  float xCoeff, yCoeff, zCoeff, zOffset; // projection info
  float fovRads, zNear, zFar; // projection base info
  int backfaces; // extra config
} BakedSettings;

typedef struct {
  int viewportWidth, viewportHeight;
  float fovDeg, zNear, zFar;
  int backfaces;
} Settings;

typedef struct {
  Float3 position, rotation, scale;
} Transform;

int max(int a, int b);
int min(int a, int b);
float Clamp01(float f);
BakedSettings BakeProjectionConstants(Settings settings);
Float3 ProjectVertex(Float3 vert, BakedSettings consts);
Float3 TransformVertex(Float3 vertex, Transform transform);
Float3 RotateEuler(Float3 vector, Float3 eulerRot);
float Float3Dot(Float3 a, Float3 b);
int Int2Cross(Int2 a, Int2 b);
Float3 SumFloat3(Float3 a, Float3 b);
Float3 SubFloat3(Float3 a, Float3 b);
Int2 SumInt2(Int2 a, Int2 b);
Int2 SubInt2(Int2 a, Int2 b);
Float3 ScaleFloat3(Float3 a, float b);
Float3 GetNormal(Float3 v1, Float3 v2, Float3 v3);
float FastInverseSqrt(float n);
Float3 NormalizeFloat3(Float3 v);

#endif