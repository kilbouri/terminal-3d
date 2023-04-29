#ifndef COLOR_H
#define COLOR_H

typedef struct {
    unsigned char r, g, b;
} Color;

#define COLOR_WHITE \
    (Color) { 255, 255, 255 }
#define COLOR_BLACK \
    (Color) { 0, 0, 0 }

void DrawColor(Color c);

#endif // COLOR_H
