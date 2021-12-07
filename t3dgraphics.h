#ifndef t3dgraphics
#define t3dgraphics

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(unix) || defined(__unix__) || defined(__unix)
#define C_CLEAR "clear"
#else
#define C_CLEAR "cls"
#endif

#define EMPTY_SPACE (' ')

typedef struct {
  int width;
  int height;
  char* colorBuffer;
} FrameBuffer;

FrameBuffer InitFrameBuffer(int w, int h);
void ClearFrameBuffer(FrameBuffer fbuf);
void FreeFrameBuffer(FrameBuffer fbuf);
void DrawFrameBuffer(FrameBuffer fbuf);
void SetPixel(FrameBuffer fbuf, int x, int y, char c);
void DrawLine(FrameBuffer fbuf, Int2 from, Int2 to, char c);
void DrawTriangle(FrameBuffer fbuf, Int2 p1, Int2 p2, Int2 p3, char c);
void FillTriangle(FrameBuffer fbuf, Int2 p1, Int2 p2, Int2 p3, char c);
#endif