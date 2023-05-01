#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <stdbool.h>
#include <math.h>

// BEHAVIOUR CONFIG
#define MAX_FPS 24
#define RECOMPUTE_NORMALS false
#define MONOSPACE_ASPECT_RATIO 2

// PROJECTION CONFIG
#define Z_FAR 10
#define Z_NEAR 0.1
#define FOV_DEGREES 80

// SHADING CONFIG
#define RENDER_WIRE_FRAME false
#define BACKFACE_CULLING true
#define SHADING_MODE SM_PHONG_SHADING

// Triangle color = surface normal
#define SM_NORMAL 0

// Triangle color proportional to triangle index (with ambient light boost)
#define SM_INDEX 1
#define INDEX_AMBIENT_LIGHT 0.125 // used only for Index-based shading

// Traingle color calculated using diffuse Phong reflection
#define SM_PHONG_SHADING 2
#define PHONG_AMBIENT_LIGHT 0.125
#define PHONG_STRENGTH 0.6
#define LIGHT_DIRECTION \
    (Vector3) { 0, -1, -1 }

// MODEL CONFIG
#define MODEL_SOURCE "models/cube.stl"
#define INITIAL_POSTION \
    (Vector3) { .x = 0, .y = 0, .z = 4 }
#define INITIAL_ROTATION_DEG \
    (Vector3) { .x = 0, .y = 0, .z = 0 }
#define INITIAL_SCALE \
    (Vector3) { .x = 1, .y = 1, .z = 1 }

#define ROTATION_PER_SECOND \
    (Vector3) { .x = 20, .y = 45, .z = 15 }

#endif // ENGINE_CONFIG_H
