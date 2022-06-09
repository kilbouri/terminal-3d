#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#define TRUE_COLOR_SUPPORT true
#define RENDER_WIRE_FRAME true
#define BACKFACE_CULLING true
#define FOV_DEGREES 90
#define Z_FAR 100
#define Z_NEAR 0.01

// how many horizontal characters does it take to take up the same width as a vertical pixel?
#define MONOSPACE_ASPECT_RATIO 1.45

// temporary model source
#define MODEL_SOURCE "./models/suzanne.stl"

#endif // ENGINE_CONFIG_H
