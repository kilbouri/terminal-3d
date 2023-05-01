#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Engine.h"

#define IsInRange(x, mi, ma) (mi <= x && x < ma)
#define ToFlat(row, col, width) (row * width + col)

float ComputeDepthLinear(ScreenPoint p1, ScreenPoint p2, ScreenPoint pActual);
float ComputeDepthTriangular(ScreenPoint p1, ScreenPoint p2, ScreenPoint p3, ScreenPoint pActual);

inline static void Swap(int* x, int* y) {
    int t = *x;
    *x = *y;
    *y = t;
}

void DrawPixel(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color) {
    // color and depth buffers should be the same size... panic if not
    assert(colorBuffer->w == depthBuffer->w);
    assert(colorBuffer->h == depthBuffer->h);

    int x = pixel.x;
    int y = pixel.y;
    float depth = pixel.depth;
    int width = colorBuffer->w;
    int height = colorBuffer->h;

    // clip pixel if needed
    if (!IsInRange(x, 0, width) || !IsInRange(y, 0, height))
        return;

    if (depth < 0) {
        // don't draw if the pixel is behind the camera
        return;
    }

    const float existingDepth = depthBuffer->contents[ToFlat(y, x, width)];
    const float depthDifference = existingDepth - depth; // when positive, the existing pixel is further away

    // Don't draw if the existing pixel is closer, OR if the difference is approximately zero (fix for edge fighting)
    if (depthDifference <= 0 || Approx(depthDifference, 0)) {
        return;
    }

    // ok, this pixel is visible on screen and closer than any other... draw it
    colorBuffer->contents[ToFlat(y, x, width)] = color;
    depthBuffer->contents[ToFlat(y, x, width)] = depth;
}

void DrawLine(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint from, ScreenPoint to, Color color) {
    // TODO: make this return an array of ScreenPoints so that FillTriangle doesn't have to iterate over a rect region
    // or define its own impl. of this method
    int x1 = from.x, y1 = from.y;
    int x2 = to.x, y2 = to.y;

    int x, y, xEnd, yEnd, i;
    int signedDX = x2 - x1;
    int signedDY = y2 - y1;

    int unsignedDX = Abs(signedDX);
    int unsignedDY = Abs(signedDY);

    int px = 2 * unsignedDY - unsignedDX;
    int py = 2 * unsignedDX - unsignedDY;

    if (unsignedDY <= unsignedDX) {
        if (signedDX >= 0) {
            x = x1;
            y = y1;
            xEnd = x2;
        } else {
            x = x2;
            y = y2;
            xEnd = x1;
        }

        ScreenPoint point = {x, y, 0};
        point.depth = ComputeDepthLinear(from, to, point);
        DrawPixel(colorBuffer, depthBuffer, point, color);

        for (i = 0; x < xEnd; i++) {
            x++;
            if (px < 0) {
                px += 2 * unsignedDY;
            } else {
                if ((0 > signedDX && signedDY < 0) || (0 < signedDX && signedDY > 0)) {
                    y++;
                } else {
                    y--;
                }

                px += 2 * (unsignedDY - unsignedDX);
            }

            ScreenPoint point = {x, y, 0};
            point.depth = ComputeDepthLinear(from, to, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
    } else {
        if (signedDY >= 0) {
            x = x1;
            y = y1;
            yEnd = y2;
        } else {
            x = x2;
            y = y2;
            yEnd = y1;
        }

        ScreenPoint point = {x, y, 0};
        point.depth = ComputeDepthLinear(from, to, point);
        DrawPixel(colorBuffer, depthBuffer, point, color);

        for (i = 0; y < yEnd; i++) {
            y++;
            if (py <= 0) {
                py += 2 * unsignedDX;
            } else {
                if ((0 > signedDX && signedDY < 0) || (0 < signedDX && signedDY > 0)) {
                    x++;
                } else {
                    x--;
                }

                py += 2 * (unsignedDX - unsignedDY);
            }

            ScreenPoint point = {x, y, 0};
            point.depth = ComputeDepthLinear(from, to, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
    }
}

void DrawWireTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color) {
    DrawLine(colorBuffer, depthBuffer, v1, v2, color);
    DrawLine(colorBuffer, depthBuffer, v2, v3, color);
    DrawLine(colorBuffer, depthBuffer, v3, v1, color);
}

void FillTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color) {
    // clang-format off
    int x1 = v1.x, y1 = v1.y;
    int x2 = v2.x, y2 = v2.y;
    int x3 = v3.x, y3 = v3.y;

    int t1x, t2x, y, t1xp, t2xp;
    int xMin, xMax;

    bool changed1 = false;
    bool changed2 = false;

    int x1Sign, x2Sign;
    int deltaX1, deltaY1;
    int deltaX2, deltaY2;
    int e1, e2;
    
    if (y1 > y2) {
        Swap(&y1, &y2);
        Swap(&x1, &x2);
    }

    if (y1 > y3) {
        Swap(&y1, &y3);
        Swap(&x1, &x3);
    }
    
    if (y2 > y3) {
        Swap(&y2, &y3);
        Swap(&x2, &x3);
    }

    t1x = t2x = x1;
    y = y1;

    deltaX1 = (int)(x2 - x1);
    if (deltaX1 < 0) {
        deltaX1 = -deltaX1;
        x1Sign = -1;
    } else {
        x1Sign = 1;
    }

    deltaY1 = (int)(y2 - y1);
    deltaX2 = (int)(x3 - x1);

    if (deltaX2 < 0) {
        deltaX2 = -deltaX2;
        x2Sign = -1;
    } else {
        x2Sign = 1;
    }

    deltaY2 = (int)(y3 - y1);

    if (deltaY1 > deltaX1) { // swap values
        Swap(&deltaX1, &deltaY1);
        changed1 = true;
    }
    
    if (deltaY2 > deltaX2) { // swap values
        Swap(&deltaY2, &deltaX2);
        changed2 = true;
    }

    e2 = (int)(deltaX2 >> 1);
    
    // "flat top" condition
    if (y1 == y2) goto secondHalf;

    e1 = (int)(deltaX1 >> 1);

    for (int i = 0; i < deltaX1;) {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x) {
            xMin = t1x;
            xMax = t2x;
        } else {
            xMin = t2x;
            xMax = t1x;
        }
        
        while (i < deltaX1) {
            i++;
            e1 += deltaY1;
            while (e1 >= deltaX1) {
                e1 -= deltaX1;
                if (changed1) t1xp = x1Sign;
                else goto upperHalfOfUpperHalf;
            }
            if (changed1) break;
            else t1x += x1Sign;
        }

    upperHalfOfUpperHalf:
        while (1) {
            e2 += deltaY2;
            while (e2 >= deltaX2) {
                e2 -= deltaX2;
                if (changed2) t2xp = x2Sign;
                else goto lowerHalfOfUpperHalf;
            }

            if (changed2) break;
            else t2x += x2Sign;
        }

    lowerHalfOfUpperHalf:
        if (xMin>t1x) xMin = t1x; 
        if (xMin>t2x) xMin = t2x;
        if (xMax<t1x) xMax = t1x; 
        if (xMax<t2x) xMax = t2x;

        for (int x = xMin; x <= xMax; x++) {
            ScreenPoint point = {x, y, 0};
            point.depth = ComputeDepthTriangular(v1, v2, v3, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
        
        if (!changed1) t1x += x1Sign;
        
        t1x += t1xp;
        
        if (!changed2) t2x += x2Sign;

        t2x += t2xp;
        y += 1;
        
        if (y == y2) break;
    }

secondHalf:
    deltaX1 = (int)(x3 - x2);
    if (deltaX1 < 0) {
        deltaX1 = -deltaX1;
        x1Sign = -1;
    } else {
        x1Sign = 1;
    }

    deltaY1 = (int)(y3 - y2);
    t1x = x2;

    if (deltaY1 > deltaX1) { // swap values
        Swap(&deltaY1, &deltaX1);
        changed1 = true;
    } else {
        changed1 = false;
    }

    e1 = (int)(deltaX1 >> 1);

    for (int i = 0; i <= deltaX1; i++) {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x) {
            xMin = t1x;
            xMax = t2x;
        } else {
            xMin = t2x;
            xMax = t1x;
        }

        // process first line until y value is about to change
        while (i < deltaX1) {
            e1 += deltaY1;
            while (e1 >= deltaX1) {
                e1 -= deltaX1;
                if (changed1) {
                    t1xp = x1Sign;
                    break;
                } else {
                    goto upperHalfOfLowerHalf;
                }
            }

            if (changed1) break;
            else t1x += x1Sign;

            if (i < deltaX1) i++;
        }
        
    upperHalfOfLowerHalf:
        while (t2x != x3) {
            e2 += deltaY2;
            while (e2 >= deltaX2) {
                e2 -= deltaX2;

                if (changed2) t2xp = x2Sign;
                else goto lowerHalfOfLowerHalf;
            }

            if (changed2) break;
            else t2x += x2Sign;
        }

    lowerHalfOfLowerHalf:
        if (xMin > t1x) xMin = t1x;
        if (xMin > t2x) xMin = t2x;
        if (xMax < t1x) xMax = t1x;
        if (xMax < t2x) xMax = t2x;

        for (int x = xMin; x <= xMax; x++) {
            ScreenPoint point = {x, y, 0};
            point.depth = ComputeDepthTriangular(v1, v2, v3, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
        
        if (!changed1) t1x += x1Sign;
        
        t1x += t1xp;
        
        if (!changed2) t2x += x2Sign;
        
        t2x += t2xp;
        y += 1;
        
        if (y > y3) return;
    }
    // clang-format on
}

float ComputeDepthLinear(ScreenPoint p1, ScreenPoint p2, ScreenPoint pActual) {
    if (p1.x == p2.x && p1.y == p2.y)
        return p1.depth;

    int vMin, vMax, vActual;

    int dX = Abs(p2.x - p1.x);
    int dY = Abs(p2.y - p1.y);

    // use the axis with a larger delta -> more precise
    if (dX > dY) {
        vMin = p1.x;
        vMax = p2.x;
        vActual = pActual.x;
    } else {
        vMin = p1.y;
        vMax = p2.y;
        vActual = pActual.y;
    }

    float t = (vActual - vMin) / (vMax - vMin);
    return Lerp(p1.depth, pActual.depth, t);
}

float ComputeDepthTriangular(ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, ScreenPoint p) {
    // unpack to floats
    float px = p.x, py = p.y;
    float v1x = v1.x, v1y = v1.y;
    float v2x = v2.x, v2y = v2.y;
    float v3x = v3.x, v3y = v3.y;

    // compute barycentric coordinates
    // https://codeplea.com/triangular-interpolation
    float denom = (v2y - v3y) * (v1x - v3x) + (v3x - v2x) * (v1y - v3y);
    denom = 1.0f / denom;

    float v1Weight = ((v2y - v3y) * (px - v3x) + (v3x - v2x) * (py - v3y)) * denom;
    float v2Weight = ((v3y - v1y) * (px - v3x) + (v1x - v3x) * (py - v3y)) * denom;
    float v3Weight = 1.0f - v1Weight - v2Weight;

    // now the depth at the point is a weighted sum of each of the vertices
    return (Abs(v1Weight) * v1.depth) + (Abs(v2Weight) * v2.depth) + (Abs(v3Weight) * v3.depth);
}
