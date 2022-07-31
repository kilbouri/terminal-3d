#include "TerminalManip.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define SHOW_CURSOR "\033[?25h"
#define HIDE_CURSOR "\033[?25l"

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
    printf("%s", visible ? SHOW_CURSOR : HIDE_CURSOR);
}

void SetCursorVisibleImm(bool visible) {
    write(
        STDOUT_FILENO,
        visible ? SHOW_CURSOR : HIDE_CURSOR,
        visible ? sizeof(SHOW_CURSOR) : sizeof(HIDE_CURSOR));
}

void CursorToHome() {
    printf("\033[H");
}
