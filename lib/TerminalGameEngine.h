#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;
struct init_bufs {
	char* screen_buf;
	float* z_buf;
};
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
 * Prepares a screen buffer and a Z-buffer of apropriate size. Returns a structure containing
 * each.
 */
struct init_bufs init() {
	char* sbuf = (char*)malloc(sizeof(char) * get_console_height() * get_console_width());
	memset(sbuf, ' ', sizeof(char) * get_console_height() * get_console_width());

	float* zbuf = (float*)calloc(get_console_height() * get_console_width(), sizeof(float));

	return (struct init_bufs) { sbuf, zbuf };
}

/**
 * A floating-point vector2
 */
struct fvector2 {
	float x;
	float y;
};

/**
 * An integer vector2
 */
struct vector2 {
	int x;
	int y;
};

/**
 * Converts a floating point normalized cartisian coordinate to
 * the screen space
 */
struct vector2 norm_to_screen(struct fvector2 coordinate) {

	float x = coordinate.x + 1;
	float y = coordinate.y + 1;

	x *= get_console_width();
	y *= get_console_height();

	return (struct vector2) { (int)x, (int)y };
}

/**
 * Ensures the point (x, y) falls within the bounds of the
 * ConsoleGameEngine's view area.
 */
void clip(int* x, int* y) {
	*x = (*x < 0) ? 0 : *x;
	*y = (*y < 0) ? 0 : *y;

	int width = get_console_width();
	int height = get_console_height();

	*x = (*x > width) ? width : *x;
	*y = (*y > height) ? height : *y;
}

/**
 * Draws a "pixel" to the console at the provided coordinate with a brightness
 * between 0 and 12.
 */
void draw(char* buffer, struct vector2 coordinate, int brightness) {
	uint screen_width = get_console_width();
	uint screen_height = get_console_height();

	int x = coordinate.x, y = coordinate.y;

	if (0 <= x && x <= screen_width && 0 <= y && y <= screen_height) {
		char brights[] = " .,-~:;=!*#$@";

		brightness = brightness < 0 ? 0 : brightness; // min
		brightness = brightness > 12 ? 12 : brightness; // max

		buffer[y * screen_width + x] = brights[brightness];
	}
}

/**
 * Fills a the box formed by the opposite corners coord1 and coord2
 */
void fill(char* buffer, struct vector2 coord1, struct vector2 coord2, int brightness) {
	int x = coord1.x; int y = coord1.y;
	int x1 = coord2.x; int y1 = coord2.y;

	clip(&x, &y);
	clip(&x1, &y1);

	if (x > x1)
		x1 = (x1 + x) - (x = x1); // swap x and x1

	if (y > y1)
		y1 = (y1 + y) - (y = y1); // swap x and x1

	for (int cx = x; cx < x1; cx++) {
		for (int cy = y; cy < y1; cy++) {
			draw(buffer, (struct vector2) { cx, cy }, brightness);
		}
	}
}

/**
 * Calls draw() to create a line in the buffer.
 *
 * Algorithm adapted from OneLoneCoder's C++ ConsoleGameEngine.
 * https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h#L477
 */
void draw_line(char* buffer, struct vector2 coord1, struct vector2 coord2, int brightness) {
	int x1 = coord1.x, x2 = coord2.x;
	int y1 = coord1.y, y2 = coord2.y;

	int x, y, xe, ye, i;
	int dx = x2 - x1;
	int dy = y2 - y1;
	
	int dx1 = abs(dx);
	int dy1 = abs(dy);

	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;

	if (dy1 <= dx1) {
		if (dx >= 0) {
			x = x1;
			y = y1;
			xe = x2;
		} else {
			x = x2;
			y = y2;
			xe = x1;
		}

		draw(buffer, (struct vector2) { x, y }, brightness);

		for (i = 0; x < xe; i++) {
			x++;
			if (px < 0)
				px += 2 * dy1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y++;
				else y--;
				
				px += 2 * (dy1 - dx1);
			}
			draw(buffer, (struct vector2) { x, y }, brightness);
		}
	} else {
		if (dy >= 0) {
			x = x1;
			y = y1;
			ye = y2;
		} else {
			x = x2;
			y = y2;
			ye = y1;
		}

		draw(buffer, (struct vector2) { x, y }, brightness);

		for (i = 0; y < ye; i++) {
			y++;
			if (py <= 0)
				py += 2 * dx1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x++;
				else x--;
				
				py += 2 * (dx1 - dy1);
			}
			draw(buffer, (struct vector2) { x, y }, brightness);
		}
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

	for (int pos = 0; pos < get_console_height() * get_console_width(); pos++) {
		putchar(buffer[pos]);
	}

	printf("\e[?25h"); // show cursor
}