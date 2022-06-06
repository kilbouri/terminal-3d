#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "TermEngine.h"

/**
 * Represents the transform of an object.
 * All values are in the global coordinate space.
 */
typedef struct {
    Vector3 position;
    Vector3 scale;
    Vector3 rotationEulerDegrees;
} Transform;

Triangle TransformTriangle(Triangle tri, Transform t);

#endif // TRANSFORM_H
