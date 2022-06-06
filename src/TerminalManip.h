#ifndef TERMINAL_MANIP_H
#define TERMINAL_MANIP_H

#include <stdbool.h>

/**
 * Enables or disables raw IO.
 */
void SetRawIO(bool);

/**
 * Hides or shows the terminal cursor.
 */
void SetCursorVisible(bool);

#endif // TERMINAL_MANIP_H
