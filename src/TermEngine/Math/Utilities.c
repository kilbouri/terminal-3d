#include "Utilities.h"

float InverseSqrt(float x) {
    long i;
    float x2, y;
    const float threeHalfs = 1.5F;

    x2 = x * 0.5F;
    y = x;
    i = *(long*)&y;            // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // what the fuck?
    y = *(float*)&i;
    y = y * (threeHalfs - (x2 * y * y));
    y = y * (threeHalfs - (x2 * y * y));
    y = y * (threeHalfs - (x2 * y * y));

    return y;
}
