#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdalign.h>

#include "../Engine.h"

typedef struct {
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
} __attribute__((aligned(16))) Transform; // align 16 for SSE

#define IdentityTransform \
    (Transform) { .position = ZerosVector3, .rotation = IdentityQuaternion, .scale = OnesVector3 }

Vector3 ApplyTransform(Vector3 vec, Transform transform);

#endif // TRANSFORM_H
