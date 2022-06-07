#include "TermMath.h"

double Lerp(double a, double b, double t) {
    return (1 - t) * a + t * b;
}

Vector3 Vector3Add(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

Vector3 Vector3Sub(Vector3 a, Vector3 b) {
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

Vector3 Vector3ScalarMul(Vector3 a, double scalar) {
    Vector3 result;
    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;

    return result;
}
