#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>

#define DEG2RAD(x) (0.01745329 * (x)) // pi / 180
#define RAD2DEG(x) (57.2957795 * (x)) // 180 / pi

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) > (b) ? (b) : (a))
#define Abs(a) ((a) > 0 ? (a) : -(a))
#define Clamp(val, min, max) (Max((min), Min((val), (max))))
#define Lerp(a, b, t) ((1.0f - (t)) * (a) + (t) * (b))
#define Approx(x, y) (Abs(x - y) <= 0.0000001f)

float InverseSqrt(float x);

#endif // UTILITIES_H
