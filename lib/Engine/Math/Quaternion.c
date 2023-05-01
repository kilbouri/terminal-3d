#include "../Engine.h"

Quaternion FromEuler(Vector3 eulerDegrees) {
    float x = DEG2RAD(eulerDegrees.x);
    float y = DEG2RAD(eulerDegrees.y);
    float z = DEG2RAD(eulerDegrees.z);

    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);

    return (Quaternion) {
        .x = sx * cy * cz + cx * sy * sz,
        .y = cx * sy * cz - sx * cy * sz,
        .z = cx * cy * sz + sx * sy * cz,
        .w = cx * cy * cz - sx * sy * sz,
    };
}

Vector3 Vector3MulQuaternion(Vector3 vec, Quaternion quat) {
    return Vector3MulMatrix3x3(vec, GetRotationMatrix(quat));
}

Quaternion MulQuaternion(Quaternion a, Quaternion b) {
    const float x = (a.x * b.w) + (a.w * b.x) + (a.y * b.z) - (a.z * b.y);
    const float y = (a.y * b.w) + (a.w * b.y) + (a.z * b.x) - (a.x * b.z);
    const float z = (a.z * b.w) + (a.w * b.z) + (a.x * b.y) - (a.y * b.x);
    const float w = (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z);

    return (Quaternion) {.x = x, .y = y, .z = z, .w = w};
}

Matrix3x3 GetRotationMatrix(Quaternion quat) {
    const float w = quat.w;
    const float x = quat.x;
    const float y = quat.y;
    const float z = quat.z;

    const float r1c1 = 1.0f - (2 * y * y) - (2 * z * z);
    const float r1c2 = (2 * x * y) - (2 * w * z);
    const float r1c3 = (2 * x * z) + (2 * w * y);

    const float r2c1 = (2 * x * y) + (2 * w * z);
    const float r2c2 = 1.0f - (2 * x * x) - (2 * z * z);
    const float r2c3 = (2 * y * z) - (2 * w * x);

    const float r3c1 = (2 * x * z) - (2 * w * y);
    const float r3c2 = (2 * y * z) + (2 * w * x);
    const float r3c3 = 1.0f - (2 * x * x) - (2 * y * y);

    return (Matrix3x3) {
        (Vector3) {.x = r1c1, .y = r1c2, .z = r1c3},
        (Vector3) {.x = r2c1, .y = r2c2, .z = r2c3},
        (Vector3) {.x = r3c1, .y = r3c2, .z = r3c3},
    };
}
