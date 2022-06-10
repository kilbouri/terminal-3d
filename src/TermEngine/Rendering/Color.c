#include <stdio.h>

#include "../Math/Utilities.h"
#include "Color.h"

void DrawColor(Color c) {
    const unsigned char r = Clamp(c.r, 0, 255);
    const unsigned char g = Clamp(c.g, 0, 255);
    const unsigned char b = Clamp(c.b, 0, 255);

    printf("\033[48;2;%u;%u;%um ", r, g, b);
}

int WriteColor(Color c, char* out) {
    const unsigned char r = Clamp(c.r, 0, 255);
    const unsigned char g = Clamp(c.g, 0, 255);
    const unsigned char b = Clamp(c.b, 0, 255);

    // 10 chars for the escape seq. + 3 for each component + 1 for space + 1 for null
    return snprintf(out, 22, "\033[48;2;%u;%u;%um ", r, g, b);
}
