#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;

struct screen_buf {
	int rows;
	int cols;
	char* chars;
};

/**
 * Stores the buffers from engine initialization
 */
struct engine_bufs {
	struct screen_buf screen;
};

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

	return (float) w.ws_row / (float) w.ws_col;
}

/**
 * Dumps the provided buffer to the console. Pads the view area as needed
 * with spaces if the provided buffer is smaller than the console's current
 * dimensions in either axis.
 */
void show(struct screen_buf buffer) {
	printf("\033[?25l"); // hide cursor
	printf("\033[H"); // return cursor to home

	for (int y = 0; y < get_console_height(); y++) {
		for (int x = 0; x < get_console_width(); x++) {
			// draw from buffer if inside view area, else draw blank
			if (y < buffer.rows && x < buffer.cols)
				putchar(buffer.chars[y * buffer.cols + x]);
			else
				putchar(' ');
		}
	}

	printf("\e[?25h"); // show cursor
}

/**
 * Prepares the needed buffers of apropriate size. Returns a structure
 * containing each of the engine's buffers.
 */
struct engine_bufs engine_init() {
	long size = get_console_height() * get_console_width();

	char* sbuf = (char*) malloc(sizeof(char) * size);
	memset(sbuf, ' ', sizeof(char) * size);

	return (struct engine_bufs) {
		(struct screen_buf) {
		.rows = get_console_height(), .cols = get_console_width(), .chars = sbuf
	}
	};
}

/**
 * Converts a floating point normalized cartisian coordinate to
 * the screen space
 */
struct vector2 norm_to_screen(struct screen_buf buffer, struct fvector2 coordinate) {

	float x = 0.5 * (coordinate.x + 1) * buffer.cols;
	float y = 0.5 * (coordinate.y + 1) * (buffer.rows - 1);

	return (struct vector2) { (int) x, (int) y };
}

/**
 * Ensures the point (x, y) falls within the bounds of the
 * view area.
 */
void clip(struct screen_buf buffer, int* x, int* y) {
	*x = (*x < 0) ? 0 : *x;
	*y = (*y < 0) ? 0 : *y;

	int width = buffer.cols;
	int height = buffer.rows;

	*x = (*x > width) ? width : *x;
	*y = (*y > height) ? height : *y;
}

/**
 * Sets a single character at the specified coordinate in the provided buffer.
 */
void set_pixel(struct screen_buf buffer, struct vector2 coordinate, char character) {
	int x = coordinate.x, y = coordinate.y;

	if (0 <= x && x <= buffer.cols && 0 <= y && y <= buffer.rows)
		buffer.chars[y * buffer.cols + x] = character;
}

/**
 * Draws a "pixel" to the console at the provided coordinate with a brightness
 * between 0 and 12.
 */
void draw(struct screen_buf buffer, struct vector2 coordinate, int brightness) {
	char brights[] = " .,-~:;=!*#$@";

	brightness = brightness < 0 ? 0 : brightness; // min
	brightness = brightness > 12 ? 12 : brightness; // max

	set_pixel(buffer, coordinate, brights[brightness]);
}

/**
 * Fills a the box formed by the opposite corners coord1 and coord2
 */
void fill_rect(struct screen_buf buffer, struct vector2 coord1, struct vector2 coord2, int brightness) {
	int x = coord1.x; int y = coord1.y;
	int x1 = coord2.x; int y1 = coord2.y;

	clip(buffer, &x, &y);
	clip(buffer, &x1, &y1);

	if (x > x1) {
		x1 += x;
		x = x1 - x;
		x1 -= x;
	}

	if (y > y1) {
		y1 += y;
		y = y1 - y;
		y1 -= y;
	}

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
void draw_line(struct screen_buf buffer, struct vector2 coord1, struct vector2 coord2, int brightness) {
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
 * Shorthand for calling draw_line() three times
 */
void draw_tri(struct screen_buf buffer, struct vector2 a, struct vector2 b, struct vector2 c, int brightness) {
	draw_line(buffer, a, b, brightness);
	draw_line(buffer, b, c, brightness);
	draw_line(buffer, c, a, brightness);
}

/**
 * Draws a filled triangle
 */
void fill_tri(struct screen_buf buffer, struct vector2 v1, struct vector2 v2, struct vector2 v3, int brightness) {

}