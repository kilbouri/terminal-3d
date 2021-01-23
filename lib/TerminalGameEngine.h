#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

/**
 * Returns the character width of the console
 */
uint get_console_width() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

/**
 * Returns the character width of the console
 */
uint get_console_height() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
}

/**
 * Returns the aspect ratio of the console
 */
float get_aspect() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return (float)w.ws_row / w.ws_col;
}

/**
 * Prepares a screen buffer of the appropriate size, filled with space
 * chars, and returns a pointer to it.
 */
char* init() {
	char* buf = (char*)malloc(sizeof(char) * get_console_height() * get_console_width());
	memset(buf, ' ', sizeof(char) * get_console_height() * get_console_width());

	return buf;
}

/**
 * Draws a "pixel" to the console at x, y with a brightness
 * between 0 and 12.
 */
void draw(char* buffer, int x, int y, int brightness) {
	uint screen_width = get_console_width();
	uint screen_height = get_console_height();

	if (0 <= x && x <= screen_width && 0 <= y && y <= screen_height) {
		char brights[] = " .,-~:;=!*#$@";

		brightness = brightness < 0 ? 0 : brightness; // min
		brightness = brightness > 12 ? 12 : brightness; // max

		buffer[y * screen_width + x] = brights[brightness];
	}
}

/**
 * Dumps the provided buffer to the console. Assumes
 * the buffer is appropriately sized for the current size
 * of the console.
 */
void show(char* buffer) {
	printf("\033[?25l"); // hide cursor
	printf("\033[H"); // return cursor to home

	uint width = get_console_width();
	for (int y = 0; y < get_console_height(); y++) {
		for (int x = 0; x < width; x++) {
			putchar(buffer[y * width + x]);
		}
	}

	printf("\e[?25h"); // show cursor
}