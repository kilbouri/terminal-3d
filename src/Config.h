#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <stdbool.h>
#include <math.h>

#define MAX_FPS 5
#define TRUE_COLOR_SUPPORT true
#define RENDER_WIRE_FRAME false
#define BACKFACE_CULLING true
#define FOV_DEGREES 80
#define Z_FAR 4
#define Z_NEAR 0.1

// how many horizontal characters does it take to take up the same width as the height of a character?
#define MONOSPACE_ASPECT_RATIO 2

#define MODEL_SOURCE "./models/cube.stl"
#define INITIAL_POSTION \
    (Vector3) { .x = 0, .y = 0, .z = 4 }
#define INITIAL_ROTATION \
    (Vector3) { .x = 0, .y = 0, .z = 0 }
#define INITIAL_SCALE \
    (Vector3) { .x = 1, .y = 1, .z = 1 }

#define ROTATION_PER_FRAME \
    (Vector3) { .x = 0, .y = M_PI_4, .z = 0 }

#endif // ENGINE_CONFIG_H
