#include <stdbool.h>

#ifndef TERMINAL_MANIP_H
#define TERMINAL_MANIP_H

/**
 * Enables or disables raw IO.
 */
void SetRawIO(bool);

/**
 * Hides or shows the terminal cursor.
 */
void SetCursorVisible(bool);

void CursorToHome();

#endif // TERMINAL_MANIP_H
