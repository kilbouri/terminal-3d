#include "t3dmath.h"

int max(int a, int b) {
  return (a > b) ? a : b;
}

int min(int a, int b) {
  return (a > b) ? b : a;
}

float Clamp01(float f) {
  f = (f < 0) ? 0 : f;
  f = (f > 1) ? 1 : f;
  return f;
}

BakedSettings BakeProjectionConstants(Settings settings) {
  // a is the aspect ratio of the screen, height/width

  // the distance scaling factor is cot(FOV/2). For user friendliness,
  //     we will treat FOV as degrees, not radians. So, we need to convert it.
  //     This factor gives the effect of distance objects being smaller, while
  //     closer objects are larger.

  float a = (float) settings.viewportHeight / settings.viewportWidth;
  float f = cot(0.5 * DEG2RAD(settings.fovDeg));
  float q = settings.zFar / (settings.zFar - settings.zNear);

  return (BakedSettings) {
    settings.viewportWidth, settings.viewportHeight,
      (a * f), f, q, (q * settings.zNear),
      DEG2RAD(settings.fovDeg), settings.zNear, settings.zFar,
      settings.backfaces
  };
}

Float3 ProjectVertex(Float3 vert, BakedSettings constants) {

  if (vert.z == 0)
    vert.z += __FLT_EPSILON__;

  float x = vert.x, y = vert.y, z = vert.z, ooz = 1 / vert.z;

  x *= constants.xCoeff * ooz;
  y *= constants.yCoeff * ooz;
  z *= constants.zCoeff;
  z -= constants.zOffset;

  return (Float3) { x, y, z };
}

Float3 TransformVertex(Float3 vertex, Transform transform) {

  // rotation
  vertex = RotateEuler(vertex, transform.rotation);

  // scale
  vertex.x *= transform.scale.x;
  vertex.y *= transform.scale.y;
  vertex.z *= transform.scale.z;

  // translation
  return SumFloat3(vertex, transform.position);
}

Float3 RotateEuler(Float3 vector, Float3 eulerRot) {
  float rX = DEG2RAD(eulerRot.x);
  float rY = DEG2RAD(eulerRot.y);
  float rZ = DEG2RAD(eulerRot.z);

  float x = vector.x;
  float y = vector.y;
  float z = vector.z;

  if (rX) {
    float c = cos(rX);
    float s = sin(rX);

    y = vector.y * c - vector.z * s;
    z = vector.y * s + vector.z * c;

    vector.y = y;
    vector.z = z;
  }

  if (rY) {
    float c = cos(rY);
    float s = sin(rY);

    x = vector.x * c + vector.z * s;
    z = vector.z * c - vector.x * s;

    vector.x = x;
    vector.z = z;
  }

  if (rZ) {
    float c = cos(rZ);
    float s = sin(rZ);

    x = vector.x * c - vector.y * s;
    y = vector.x * s + vector.y * c;

    vector.x = x;
    vector.y = y;
  }

  return vector;
}

float Float3Dot(Float3 a, Float3 b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

int Int2Cross(Int2 a, Int2 b) {
  return a.x * b.y - a.y * b.x;
}

Float3 SumFloat3(Float3 a, Float3 b) {
  return (Float3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

Float3 SubFloat3(Float3 a, Float3 b) {
  return (Float3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Int2 SumInt2(Int2 a, Int2 b) {
  return (Int2) { a.x + b.x, a.y + b.y };
}

Int2 SubInt2(Int2 a, Int2 b) {
  return (Int2) { a.x - b.x, a.y - b.y };
}

Float3 ScaleFloat3(Float3 a, float b) {
  return (Float3) { a.x* b, a.y* b, a.z* b };
}

Float3 GetNormal(Float3 v1, Float3 v2, Float3 v3) {
  Float3 line1 = SubFloat3(v2, v1);
  Float3 line2 = SubFloat3(v3, v1);

  Float3 normal = { // cross product
    (line1.y * line2.z) - (line1.z * line2.y),
    (line1.z * line2.x) - (line1.x * line2.z),
    (line1.x * line2.y) - (line1.y * line2.x),
  };

  return normal;
}

float FastInverseSqrt(float n) {
  const float threehalfs = 1.5f;

  float y = n;
  long i = *(long*) &y;

  i = 0x5f3759df - (i >> 1);
  y = *(float*) &i;
  y = y * (threehalfs - (n * 0.5f * y * y));

  return y;
}

Float3 NormalizeFloat3(Float3 v) {
  float magnitude = v.x * v.x + v.y * v.y + v.z * v.z;
  return ScaleFloat3(v, FastInverseSqrt(magnitude));
}
