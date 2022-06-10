#ifndef COLOR_H
#define COLOR_H

typedef struct {
    unsigned char r, g, b;
} Color;

#define COLOR_WHITE \
    (Color) { 255, 255, 255 }
#define COLOR_BLACK \
    (Color) { 0, 0, 0 }

Color ClampColor(Color c);
void DrawColor(Color c);

/**
 * Write a color to a string. Guaranteed to write at most 22 characters (null included).
 * Return code matches snprintf (used internally)
 */
int WriteColor(Color c, char* out);

#endif // COLOR_H
