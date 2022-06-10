#include "TerminalManip.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void SetRawInput(bool enabled) {
    static struct termios originalIn;
    static bool rawModeSet = false;

    if (enabled && !rawModeSet) {
        // raw input (un-buffered, un-echoed)
        tcgetattr(STDIN_FILENO, &originalIn);
        struct termios modifiedIn = originalIn;

        modifiedIn.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &modifiedIn);

        rawModeSet = true;
    } else if (!enabled && rawModeSet) {
        // return to original input and output modes
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalIn);
    }
}

void WriteOutputBuffer(char* outputBuffer, int numBytes) {
    fwrite(outputBuffer, numBytes, 1, stdout);
}

void SetCursorVisible(bool visible) {
    printf(visible ? "\033[?25h" : "\033[?25l");
}

void CursorToHome() {
    printf("\033[H");
}
