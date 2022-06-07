#ifndef TERM_MATH_H
#define TERM_MATH_H

#include "TermEngine.h"

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) > (b) ? (b) : (a))
#define Abs(a) ((a) > 0 ? (a) : -(a))

double Lerp(double a, double b, double t);
Vector3 Vector3Add(Vector3 a, Vector3 b);
Vector3 Vector3Sub(Vector3 a, Vector3 b);
Vector3 Vector3ScalarMul(Vector3 a, double scalar);

#endif // TERM_MATH_H
