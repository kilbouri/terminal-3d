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
 * Determines x at y given two points (start and end) on a line
 */
float linear_interpolate(struct vector2 start, struct vector2 end, int y) {
	int x1 = start.x, y1 = start.y;
	int x2 = end.x, y2 = end.y;

	int dy = y2 - y1;
	int dx = x2 - x1;

	return x1 + ((y - y1) * dx / (dy ? dy : 1));
}

/**
 * Converts a floating point normalized cartisian coordinate to
 * the screen space
 */
struct vector2 norm_to_screen(struct fvector2 coordinate) {

	float x = 0.5 * (coordinate.x + 1);
	float y = 0.5 * (coordinate.y + 1);

	x *= get_console_width();
	y *= get_console_height() - 1;

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
void fill_rect(char* buffer, struct vector2 coord1, struct vector2 coord2, int brightness) {
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
 * Shorthand for calling draw_line() three times
 */
void draw_tri(char* buffer, struct vector2 a, struct vector2 b, struct vector2 c, int brightness) {
	draw_line(buffer, a, b, brightness);
	draw_line(buffer, b, c, brightness);
	draw_line(buffer, c, a, brightness);
}

/**
 * Draws a filled triangle
 */
void fill_tri(char* buffer, struct vector2 a, struct vector2 b, struct vector2 c, int brightness) {
	// sort so that a, b, and c are in order, vertically.
	struct vector2 tempCoord;
	if (b.y < a.y) {
		// swap a and b
		tempCoord = a;
		a = b;
		b = tempCoord;
	}
	if (c.y < a.y) {
		// swap c and a
		tempCoord = c;
		c = a;
		a = tempCoord;
	}
	if (c.y < b.y) {
		// swap c and b
		tempCoord = c;
		c = b;
		b = tempCoord;
	}

	for (int y = a.y; y <= c.y; y++) {
		int x1, x2;
		if (y <= b.y) { // 'top' half
			x1 = (int)linear_interpolate(a, b, y - a.y);
			x2 = (int)linear_interpolate(a, c, y - a.y);
		} else { // 'bottom' half
			x1 = (int)linear_interpolate(b, c, y - a.y);
			x2 = (int)linear_interpolate(a, c, y - a.y);
		}

		struct vector2 p1 = { x1, y };
		struct vector2 p2 = { x2, y };

		draw_line(buffer, p1, p2, brightness);
	}

	draw_tri(buffer, a, b, c, brightness - 5);
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