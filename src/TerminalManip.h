#include <stdbool.h>

#ifndef TERMINAL_MANIP_H
#define TERMINAL_MANIP_H

void SetRawInput(bool);
void WriteOutputBuffer(char* outputBuffer, int numBytes);
void SetCursorVisible(bool visible);
void CursorToHome();

#endif // TERMINAL_MANIP_H
