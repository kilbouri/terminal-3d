#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

typedef struct {
    Vector3 row1, row2, row3;
} Matrix3x3;

typedef struct {
    Vector4 row1, row2, row3, row4;
} Matrix4x4;

#define ZerosMatrix3x3 \
    (Matrix3x3) { ZerosVector3, ZerosVector3, ZerosVector3 }
#define OnesMatrix3x3 \
    (Matrix3x3) { OnesVector3, OnesVector3, OnesVector3 }

#define ZerosMatrix4x4 \
    (Matrix4x4) { ZerosVector4, ZerosVector4, ZerosVector4, ZerosVector4 }
#define OnesMatrix4x4 \
    (Matrix4x4) { OnesVector4, OnesVector4, OnesVector4, OnesVector4 }

Vector3 Vector3MulMatrix3x3(Vector3 vec, Matrix3x3 mat);
Vector3 GetColumnMatrix3x3(int column, Matrix3x3 mat);

Vector4 Vector4MulMatrix4x4(Vector4 vec, Matrix4x4 mat);
Vector4 GetColumnMatrix4x4(int column, Matrix4x4 mat);

#endif // MATRIX_H
