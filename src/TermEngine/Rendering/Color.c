#include <stdio.h>

#include "Color.h"

void DrawColor(Color c) {
    // Sets background color in RGB, then prints a space.
    printf("\033[48;2;%u;%u;%um ", c.r, c.g, c.b);
}
