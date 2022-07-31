#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdalign.h>

#include "Vector.h"
#include "Quaternion.h"

typedef struct {
    Vector3 position;
    Vector3 scale;
    Quaternion rotation;
} __attribute__((aligned(16))) Transform; // align 16 for SSE

#define IdentityTransform \
    { ZerosVector3, IdentityQuaternion, OnesVector3 }

Vector3 ApplyTransform(Vector3 vec, Transform transform);

#endif // TRANSFORM_H
