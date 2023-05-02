#include <stdio.h>

#include "../Engine.h"
#include "Color.h"

#define CHANNEL_FROM_01(val) \
    ((COLOR_CHANNEL_MAX - COLOR_CHANNEL_MIN) * (val) + COLOR_CHANNEL_MIN)

inline static float HSVChannelFunction(float hue, float sat, float val, float n) {
    const float k = fmodf(n + hue / 60, 6);
    return val - (val * sat * Max(0, Min(4 - k, 1)));
}

Color ColorFromHSV(float hue, float sat, float val) {
    const float h = Clamp(hue, 0, 360);
    const float s = Clamp(sat, 0, 1);
    const float v = Clamp(val, 0, 1);

    return (Color) {
        .r = CHANNEL_FROM_01(HSVChannelFunction(h, s, v, 5)),
        .g = CHANNEL_FROM_01(HSVChannelFunction(h, s, v, 3)),
        .b = CHANNEL_FROM_01(HSVChannelFunction(h, s, v, 1)),
    };
}

Color ColorFromRGB(float red, float green, float blue) {
    const float r = Clamp(red, 0, 1);
    const float g = Clamp(green, 0, 1);
    const float b = Clamp(blue, 0, 1);

    return (Color) {
        .r = CHANNEL_FROM_01(r),
        .g = CHANNEL_FROM_01(g),
        .b = CHANNEL_FROM_01(b),
    };
}

bool ColorEquals(Color a, Color b) {
    // clang-format off
    return (a.r == b.r)
        && (a.g == b.g) 
        && (a.b == b.b);
    // clang-format on
}

void WriteColor(Color c) {
    const unsigned char r = Clamp(c.r, COLOR_CHANNEL_MIN, COLOR_CHANNEL_MAX);
    const unsigned char g = Clamp(c.g, COLOR_CHANNEL_MIN, COLOR_CHANNEL_MAX);
    const unsigned char b = Clamp(c.b, COLOR_CHANNEL_MIN, COLOR_CHANNEL_MAX);

    printf("\033[48;2;%u;%u;%um ", r, g, b);
}
