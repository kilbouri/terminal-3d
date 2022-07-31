#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include "Matrix.h"

typedef Vector4 Quaternion;

#define IdentityQuaternion \
    { 0, 0, 0, 1 }

Quaternion FromEuler(Vector3 eulerDegrees);
Quaternion MulQuaternion(Quaternion a, Quaternion b);

Vector3 Vector3MulQuaternion(Vector3 vec, Quaternion quat);
Matrix3x3 GetRotationMatrix(Quaternion quat);

#endif // QUATERNION_H
