#include "Vector.h"
#include <assert.h>

Vector3 AddVector3(Vector3 a, Vector3 b) {
    return (Vector3) {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

Vector3 SubVector3(Vector3 a, Vector3 b) {
    return (Vector3) {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

Vector3 MulVector3(Vector3 vec, float scalar) {
    return (Vector3) {
        .x = vec.x * scalar,
        .y = vec.y * scalar,
        .z = vec.z * scalar,
    };
}

Vector3 CrossVector3(Vector3 a, Vector3 b) {
    return (Vector3) {
        .x = (a.y * b.z) - (a.z * b.y),
        .y = (a.z * b.x) - (a.x * b.z),
        .z = (a.x * b.y) - (a.y * b.x),
    };
}

float DotVector3(Vector3 a, Vector3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector4 AddVector4(Vector4 a, Vector4 b) {
    return (Vector4) {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w,
    };
}

Vector4 SubVector4(Vector4 a, Vector4 b) {
    return (Vector4) {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w,
    };
}

Vector4 MulVector4(Vector4 vec, float scalar) {
    return (Vector4) {
        .x = vec.x * scalar,
        .y = vec.y * scalar,
        .z = vec.z * scalar,
        .w = vec.w * scalar,
    };
}

float DotVector4(Vector4 a, Vector4 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}
