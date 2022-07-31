#include "Utilities.h"

float InverseSqrt(float x) {
    union {
        float f;
        long i;
    } convert;

    float halfX = 0.5f * x;

    convert.f = x;
    convert.i = 0x5f3759df - (convert.i >> 1);
    convert.f = convert.f * (1.5f - (halfX * convert.f * convert.f));

    return convert.f;
}
