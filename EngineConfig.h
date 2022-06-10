#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#define MAX_FPS 30
#define TRUE_COLOR_SUPPORT true
#define RENDER_WIRE_FRAME true
#define BACKFACE_CULLING true
#define FOV_DEGREES 90
#define Z_FAR 100
#define Z_NEAR 0.01

// how many horizontal characters does it take to take up the same width as a vertical pixel?
#define MONOSPACE_ASPECT_RATIO 1.45

// temporary model source
// #define MODEL_SOURCE "./models/tank.stl"
// #define INITIAL_POSTION \
//     (Vector3) { .x = 0, .y = 0.45, .z = 1 }
// #define INITIAL_ROTATION \
//     (Vector3) { .x = 180, .y = 180, .z = 0 }
// #define INITIAL_SCALE \
//     (Vector3) { .x = 1, .y = 1, .z = 1 }

// #define ROTATION_PER_FRAME \
//     (Vector3) { .x = 0, .y = 10, .z = 0 }

// #define MODEL_SOURCE "./models/suzanne.stl"
// #define INITIAL_POSTION \
//     (Vector3) { .x = 0, .y = 0, .z = 2.2 }
// #define INITIAL_ROTATION \
//     (Vector3) { .x = 90, .y = 180, .z = 180 }
// #define INITIAL_SCALE \
//     (Vector3) { .x = 1, .y = 1, .z = 1 }

// #define ROTATION_PER_FRAME \
//     (Vector3) { .x = 0, .y = 0, .z = 10 }

#define MODEL_SOURCE "./models/gem.stl"
#define INITIAL_POSTION \
    (Vector3) { .x = 0, .y = 0, .z = 1.5 }
#define INITIAL_ROTATION \
    (Vector3) { .x = -115, .y = 0, .z = 0 }
#define INITIAL_SCALE \
    (Vector3) { .x = 1, .y = 1, .z = 1 }

#define ROTATION_PER_FRAME \
    (Vector3) { .x = 0, .y = 0, .z = 5 }

// #define MODEL_SOURCE "./models/cube.stl"
// #define INITIAL_POSTION \
//     (Vector3) { .x = 0, .y = 0, .z = 3 }
// #define INITIAL_ROTATION \
//     (Vector3) { .x = 0, .y = 0, .z = 0 }
// #define INITIAL_SCALE \
//     (Vector3) { .x = 1, .y = 1, .z = 1 }

// #define ROTATION_PER_FRAME \
//     (Vector3) { .x = 5, .y = 5, .z = 10 }

#endif // ENGINE_CONFIG_H
