#ifndef COLOR_H
#define COLOR_H

typedef struct {
    unsigned char r, g, b;
} Color;

#define COLOR_CHANNEL_MAX 255
#define COLOR_CHANNEL_MIN 0

#define COLOR_WHITE \
    (Color) { COLOR_CHANNEL_MAX, COLOR_CHANNEL_MAX, COLOR_CHANNEL_MAX }
#define COLOR_BLACK \
    (Color) { COLOR_CHANNEL_MIN, COLOR_CHANNEL_MIN, COLOR_CHANNEL_MIN }

Color ColorFromHSV(float hue, float sat, float val);
Color ColorFromRGB(float red, float green, float blue);
void WriteColor(Color c);

#endif // COLOR_H
