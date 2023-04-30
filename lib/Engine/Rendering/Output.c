#include <stdio.h>
#include <assert.h>

#include "../Engine.h"

void Render(ColorBuffer* colorBuffer) {
    int numElements = colorBuffer->w * colorBuffer->h;
    for (int i = 0; i < numElements; i++) {
        WriteColor(colorBuffer->contents[i]);
    }
}

void RenderDepth(DepthBuffer* depthBuffer, float zNear, float zFar) {
    int numElements = depthBuffer->w * depthBuffer->h;
    float denom = 1.0f / (zFar - zNear);

    for (int i = 0; i < numElements; i++) {
        float rawDepth01 = (depthBuffer->contents[i] - zNear) * denom;
        float clampedDepth01 = Clamp(rawDepth01, 0, 1);
        float brightness = clampedDepth01 * COLOR_CHANNEL_MAX;

        Color color = (Color) {
            .r = brightness,
            .g = brightness,
            .b = brightness,
        };

        WriteColor(color);
    }
}
