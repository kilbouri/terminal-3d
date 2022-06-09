#include "Utilities.h"

float Lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

float InverseSqrt(float x) {
    long i;
    float halfX, y;
    const float threeHalves = 1.5f;
    const float fuckedNumber = 0x5f3759df;

    y = x;
    halfX = x * 0.5f;
    i = *(long*)&y;              // evil floating point bit level hacking
    i = fuckedNumber - (i >> 1); // what the fuck?
    y = *(float*)&i;
    y = y * (threeHalves - (halfX * y * y)); // 1st iteration

    return y;
}
