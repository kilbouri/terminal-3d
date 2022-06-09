#include "assert.h"
#include "Matrix.h"
#include "Vector.h"

Vector3 Vector3MulMatrix3x3(Vector3 vec, Matrix3x3 mat) {
    return (Vector3) {
        .x = DotVector3(vec, GetColumnMatrix3x3(1, mat)),
        .y = DotVector3(vec, GetColumnMatrix3x3(2, mat)),
        .z = DotVector3(vec, GetColumnMatrix3x3(3, mat)),
    };
}

Vector3 GetColumnMatrix3x3(int column, Matrix3x3 mat) {
    assert(1 <= column && column <= 3);

    return // clang-format off
    (column == 1) ? (Vector3) {mat.row1.x, mat.row2.x, mat.row3.x} :
    (column == 2) ? (Vector3) {mat.row1.y, mat.row2.y, mat.row3.y} :
                    (Vector3) {mat.row1.z, mat.row2.z, mat.row3.z};
    // clang-format on
}

Vector4 Vector4MulMatrix4x4(Vector4 vec, Matrix4x4 mat) {
    return (Vector4) {
        .x = DotVector4(vec, GetColumnMatrix4x4(1, mat)),
        .y = DotVector4(vec, GetColumnMatrix4x4(2, mat)),
        .z = DotVector4(vec, GetColumnMatrix4x4(3, mat)),
        .w = DotVector4(vec, GetColumnMatrix4x4(4, mat)),
    };
}

Vector4 GetColumnMatrix4x4(int column, Matrix4x4 mat) {
    assert(1 <= column && column <= 4);

    return // clang-format off
    (column == 1) ? (Vector4) {mat.row1.x, mat.row2.x, mat.row3.x, mat.row4.x} :
    (column == 2) ? (Vector4) {mat.row1.y, mat.row2.y, mat.row3.y, mat.row4.y} :
    (column == 3) ? (Vector4) {mat.row1.z, mat.row2.z, mat.row3.z, mat.row4.z} :
                    (Vector4) {mat.row1.w, mat.row2.w, mat.row3.w, mat.row4.w};
    // clang-format on
}
