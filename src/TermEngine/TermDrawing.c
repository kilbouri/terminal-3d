#include <assert.h>

#include "TermEngine.h"
#include "TermMath.h"

#define IsInRange(x, mi, ma) (mi <= x && x < ma)
#define ToFlat(row, col, width) (row * width + col)

void DrawPixel(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color) {
    // color and depth buffers should be the same size... panic if not
    assert(colorBuffer->w == depthBuffer->w);
    assert(colorBuffer->h == depthBuffer->h);

    int x = pixel.x;
    int y = pixel.y;
    double depth = pixel.depth;
    int width = colorBuffer->w;
    int height = colorBuffer->h;

    // clip pixel if needed
    if (!IsInRange(x, 0, width) || !IsInRange(y, 0, height)) return;

    // depth check
    if (depth >= depthBuffer->contents[ToFlat(y, x, width)]) return;

    // ok, this pixel is visible on screen and closer than any other... draw it
    colorBuffer->contents[ToFlat(y, x, width)] = color;
    depthBuffer->contents[ToFlat(y, x, width)] = depth;
}

double ComputeDepth(ScreenPoint p1, ScreenPoint p2, ScreenPoint pActual) {
    if (p1.x == p2.x && p1.y == p2.y) return p1.depth;

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

    double t = (vActual - vMin) / (vMax - vMin);
    return Lerp(p1.depth, pActual.depth, t);
}

void DrawLine(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint from, ScreenPoint to, Color color) {
    // TODO: make this return an array of ScreenPoints so that FillTriangle doesn't have to iterate over a rect region
    // or define its own impl. of this method
    int x1 = from.x, y1 = from.y;
    int x2 = to.x, y2 = to.y;

    int x, y, xEnd, yEnd, i;
    int usdx = x2 - x1;
    int usdy = y2 - y1;

    int sdx = Abs(usdx);
    int sdy = Abs(usdy);

    int px = 2 * sdy - sdx;
    int py = 2 * sdx - sdy;

    if (sdy <= sdx) {
        if (usdx >= 0) {
            x = x1;
            y = y1;
            xEnd = x2;
        } else {
            x = x2;
            y = y2;
            xEnd = x1;
        }

        ScreenPoint point = { x, y, 0 };
        point.depth = ComputeDepth(from, to, point);
        DrawPixel(colorBuffer, depthBuffer, point, color);

        for (i = 0; x < xEnd; i++) {
            x++;
            if (px < 0)
                px += 2 * sdy;
            else {
                if ((usdx < 0 && usdy < 0) || (usdx > 0 && usdy > 0)) y++;
                else y--;

                px += 2 * (sdy - sdx);
            }

            ScreenPoint point = { x, y, 0 };
            point.depth = ComputeDepth(from, to, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
    } else {
        if (usdy >= 0) {
            x = x1;
            y = y1;
            yEnd = y2;
        } else {
            x = x2;
            y = y2;
            yEnd = y1;
        }

        ScreenPoint point = { x, y, 0 };
        point.depth = ComputeDepth(from, to, point);
        DrawPixel(colorBuffer, depthBuffer, point, color);

        for (i = 0; y < yEnd; i++) {
            y++;
            if (py <= 0)
                py += 2 * sdx;
            else {
                if ((usdx < 0 && usdy < 0) || (usdx > 0 && usdy > 0)) x++;
                else x--;

                py += 2 * (sdx - sdy);
            }

            ScreenPoint point = { x, y, 0 };
            point.depth = ComputeDepth(from, to, point);
            DrawPixel(colorBuffer, depthBuffer, point, color);
        }
    }
}

void DrawWireTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint v1, ScreenPoint v2, ScreenPoint v3, Color color) {
    DrawLine(colorBuffer, depthBuffer, v1, v2, color);
    DrawLine(colorBuffer, depthBuffer, v2, v3, color);
    DrawLine(colorBuffer, depthBuffer, v3, v1, color);
}

void FillTriangle(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer, ScreenPoint pixel, Color color) {

}
