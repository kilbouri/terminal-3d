#include <stdio.h>

#include "../Engine.h"

void WriteColor(Color c) {
    const unsigned char r = Clamp(c.r, 0, 255);
    const unsigned char g = Clamp(c.g, 0, 255);
    const unsigned char b = Clamp(c.b, 0, 255);

    printf("\033[48;2;%u;%u;%um ", r, g, b);
}
