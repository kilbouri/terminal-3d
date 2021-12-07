#include <stdlib.h>
#include "t3dmath.h"
#include "t3dgraphics.h"

FrameBuffer InitFrameBuffer(int w, int h) {
    char* pixels = (char*)malloc(w * h * sizeof(char));
    for (int i = 0; i < w * h; i++)
        pixels[i] = EMPTY_SPACE;

    return (FrameBuffer) { w, h, pixels };
}

void ClearFrameBuffer(FrameBuffer fbuf) {
    for (int y = 0; y < fbuf.height; y++)
        for (int x = 0; x < fbuf.width; x++)
            fbuf.colorBuffer[fbuf.width * y + x] = EMPTY_SPACE;
}

void FreeFrameBuffer(FrameBuffer fbuf) {
    free(fbuf.colorBuffer);
}

void DrawFrameBuffer(FrameBuffer fbuf) {
    fputs("\e[0m\e[H", stdout); // -> success!
    // fputs("\x1b[0;0H", stdout); // -> success!

    for (int y = 0; y < fbuf.height; y++) {
        fwrite(fbuf.colorBuffer + (fbuf.width * y), sizeof(char), fbuf.width, stdout);
        fputc('\n', stdout);
    }
}

void SetPixel(FrameBuffer fbuf, int x, int y, char c) {
    if (0 <= x && x < fbuf.width && 0 <= y && y < fbuf.height) {
        fbuf.colorBuffer[fbuf.width * y + x] = c;
    }
}

void DrawLine(FrameBuffer fbuf, Int2 from, Int2 to, char c) {
    int x2 = to.x, y2 = to.y;
    int x1 = from.x, y1 = from.y;

    int x, y, xend, yend, i;
    int usdx = x2 - x1;
    int usdy = y2 - y1;

    int sdx = abs(usdx);
    int sdy = abs(usdy);

    int px = 2 * sdy - sdx;
    int py = 2 * sdx - sdy;

    if (sdy <= sdx) {
        if (usdx >= 0) {
            x = x1;
            y = y1;
            xend = x2;
        }
        else {
            x = x2;
            y = y2;
            xend = x1;
        }

        SetPixel(fbuf, x, y, c);

        for (i = 0; x < xend; i++) {
            x++;
            if (px < 0)
                px += 2 * sdy;
            else {
                if ((usdx < 0 && usdy < 0) || (usdx > 0 && usdy > 0)) y++;
                else y--;

                px += 2 * (sdy - sdx);
            }

            SetPixel(fbuf, x, y, c);
        }
    }
    else {
        if (usdy >= 0) {
            x = x1;
            y = y1;
            yend = y2;
        }
        else {
            x = x2;
            y = y2;
            yend = y1;
        }

        SetPixel(fbuf, x, y, c);

        for (i = 0; y < yend; i++) {
            y++;
            if (py <= 0)
                py += 2 * sdx;
            else {
                if ((usdx < 0 && usdy < 0) || (usdx > 0 && usdy > 0)) x++;
                else x--;

                py += 2 * (sdx - sdy);
            }

            SetPixel(fbuf, x, y, c);
        }
    }
}

void DrawTriangle(FrameBuffer fbuf, Int2 p1, Int2 p2, Int2 p3, char c) {
    DrawLine(fbuf, p1, p2, c);
    DrawLine(fbuf, p2, p3, c);
    DrawLine(fbuf, p3, p1, c);
}

void FillTriangle(FrameBuffer fbuf, Int2 p1, Int2 p2, Int2 p3, char c) {

    // scanline fill algo:
    // define a rectangular region R with diagonal corners (x1, y1) and (x2, y2)
    //  where: x1 = min(p1.x, p2.x, p3.x)
    //         x2 = max(p1.x, p2.x, p3.x)
    //         y1 = min(p1.y, p2.y, p3.y)
    //         y2 = max(p1.y, p2.y, p3.y)
    // iterate along the y axis from y1 to y2.
    //  iterate along the x axis from x1 to x2
    //   check if (x, y) is inside the triangle using cross products
    //    draw pixel if it is

    int x1 = min(p1.x, min(p2.x, p3.x));
    int x2 = max(p1.x, max(p2.x, p3.x));
    int y1 = min(p1.y, min(p2.y, p3.y));
    int y2 = max(p1.y, max(p2.y, p3.y));

    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            // let P denote (x, y). Then
            // if p1P x p1p2, p2P x p2p3, and p3P x p3p1 all have matching signs,
            //    one is zero and the other two match signs, or two are zero,
            // then P is inside the triangle
            Int2 P = { x, y };

            // vectors needed in the cross product
            Int2 p1P = SubInt2(P, p1), p1p2 = SubInt2(p2, p1);
            Int2 p2P = SubInt2(P, p2), p2p3 = SubInt2(p3, p2);
            Int2 p3P = SubInt2(P, p3), p3p1 = SubInt2(p1, p3);

            // cross products
            int p1Pxp1p2 = Int2Cross(p1P, p1p2);
            int p2Pxp2p3 = Int2Cross(p2P, p2p3);
            int p3Pxp3p1 = Int2Cross(p3P, p3p1);

            // count the number of 0s, -ves, and +ves
            int zeros = (p1Pxp1p2 == 0) + (p2Pxp2p3 == 0) + (p3Pxp3p1 == 0);
            int positives = (p1Pxp1p2 > 0) + (p2Pxp2p3 > 0) + (p3Pxp3p1 > 0);
            int negatives = (p1Pxp1p2 < 0) + (p2Pxp2p3 < 0) + (p3Pxp3p1 < 0);

            if ((zeros + positives == 3) || (zeros + negatives == 3) || zeros == 2)
                SetPixel(fbuf, x, y, c);
        }
    }
}