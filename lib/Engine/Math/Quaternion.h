#ifndef QUATERNION_H
#define QUATERNION_H

#include "../Engine.h"

typedef Vector4 Quaternion;

#define IdentityQuaternion \
    (Quaternion) { .x = 0, .y = 0, .z = 0, .w = 1 }

Quaternion FromEuler(Vector3 eulerDegrees);
Quaternion MulQuaternion(Quaternion a, Quaternion b);

Vector3 Vector3MulQuaternion(Vector3 vec, Quaternion quat);
Matrix3x3 GetRotationMatrix(Quaternion quat);

#endif // QUATERNION_H
