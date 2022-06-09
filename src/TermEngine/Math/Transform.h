#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector.h"
#include "Quaternion.h"

typedef struct {
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
} Transform;

Vector3 ApplyTransform(Vector3 vec, Transform transform);

#endif // TRANSFORM_H
