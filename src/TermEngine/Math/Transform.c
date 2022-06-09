#include "Transform.h"

Vector3 ApplyTransform(Vector3 vec, Transform transform) {
    Vector3 rotated = Vector3MulQuaternion(vec, transform.rotation);

    Vector3 scaled = {
        .x = rotated.x * transform.scale.x,
        .y = rotated.y * transform.scale.y,
        .z = rotated.z * transform.scale.z,
    };

    Vector3 translated = AddVector3(scaled, transform.position);
    return translated;
}
