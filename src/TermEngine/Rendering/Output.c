#include "Buffer.h"
#include "Color.h"

void Render(ColorBuffer* colorBuffer) {
    int numElements = colorBuffer->w * colorBuffer->h;
    for (int i = 0; i < numElements; i++) {
        DrawColor(colorBuffer->contents[i]);
    }
}
