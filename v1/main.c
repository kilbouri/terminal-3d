#include "./lib/MatLib.c"
#include "./lib/STLMesh.c"
#include "./lib/Utilities.c"
#include "./lib/TerminalGraphics.c"

struct Mat* get_triangle_matrix(struct Mat*, struct Mat*, struct Mat*, struct Mat*);
struct Mat* project_screen(struct Mat*);
struct fvector2 mat_to_fvector(struct Mat*);
struct Mat* get_rotation_matrix(float, float);

/**
 * For now, these definitions control parameters
 * used in the rendering pipeline.
 *
 * fNEAR 				- near clipping plane
 * fFAR 				- far clipping plane
 * FRUSTUM 			- the camera view plane calculation
 * FOV 					- the camera's field of view. Integer multiples of 180 are bad news!
 * DIST_CORRECTION 		- the distance correction calculation
 * TERMINAL_CORRECTION	- scales the vertical axis to fix vertical stretching in the final rasterization
 * [XY]_ROT_SPEED		- how fast should the mesh rotate? (measured in radians/frame) TODO: Normalize framerate to max of 30fps
 * SCALE				- scales the model evenly along all axes
 */
#define fNEAR 0.1
#define fFAR 1000
#define FRUSTUM (fFAR / (fFAR - fNEAR))
#define FOV 90
#define DIST_CORRECTION (1.0 / tan(FOV * 0.5 * (3.1415926 / 180.0)))
#define TERMINAL_CORRECTION 0.5
#define X_ROT_SPEED 0.0125
#define Y_ROT_SPEED 0.015
#define SCALE 1

int main(int argc, const char* argv[]) {
    FILE* model_src;
    if (argc >= 2) {
        model_src = fopen(argv[1], "r");
    } else {
        puts("Defaulting to /models/cube.stl...");
        model_src = fopen("./models/cube.stl", "r");
    }

    mesh model = load_stl(model_src);
    fclose(model_src);

    // ----------------- 3D RENDERER --------------------
    struct engine_bufs buffers = engine_init();

    /**
     * Render loop
     */
    float x_rot = -M_PI / 8;
    float y_rot = 0;

    while (1) {

        // reset buffer (keep as close to computation loop as possible to minimize flashing!)
        fill_rect(buffers.screen, (struct vector2) { 0, 0 }, (struct vector2) { buffers.screen.cols, buffers.screen.rows }, 0);

        // get this frame's rotation matrix
        struct Mat* rotation = get_rotation_matrix(x_rot, y_rot);
        for (int i = 0; i < model.tri_count; i++) {
            struct Mat* scaled = scalermultiply(model.triangles[i], SCALE);
            struct Mat* rotated = multiply(scaled, rotation);
            struct Mat* projected = project_screen(rotated);

            struct fvector2 vec_a = { mat_get(projected, 2, 1), mat_get(projected, 2, 2) };
            struct fvector2 vec_b = { mat_get(projected, 3, 1), mat_get(projected, 3, 2) };
            struct fvector2 vec_c = { mat_get(projected, 4, 1), mat_get(projected, 4, 2) };

            struct vector2 v1 = norm_to_screen(buffers.screen, vec_a);
            struct vector2 v2 = norm_to_screen(buffers.screen, vec_b);
            struct vector2 v3 = norm_to_screen(buffers.screen, vec_c);

            // draw calls
            draw_tri(buffers.screen, v1, v2, v3, 9);

            // garbage collection
            freemat(scaled);
            freemat(rotated);
        }

        // display
        show(buffers.screen);

        x_rot += X_ROT_SPEED;
        y_rot += Y_ROT_SPEED;

        freemat(rotation);
    }
}

/**
 * Applies the projection matrix to the provided coordinate.
 */
struct Mat* project_screen(struct Mat* face) {
    struct Mat* projection_matrix = zeros(4, 4);

    mat_set(projection_matrix, 1, 1, DIST_CORRECTION * get_aspect());
    mat_set(projection_matrix, 2, 2, DIST_CORRECTION * TERMINAL_CORRECTION);
    mat_set(projection_matrix, 3, 3, FRUSTUM);
    mat_set(projection_matrix, 4, 3, -fNEAR * FRUSTUM);
    mat_set(projection_matrix, 3, 4, 1);

    struct Mat* translated = copyvalue(face);
    mat_set(translated, 1, 3, mat_get(translated, 1, 3) + 3); // pushes all meshes into the screen by 3 units

    struct Mat* projected = multiply(translated, projection_matrix);
    if (!mat_get(projected, 1, 4)) {
        mat_set(projected, 1, 1, mat_get(projected, 1, 1) / mat_get(projected, 1, 4));
        mat_set(projected, 1, 2, mat_get(projected, 1, 2) / mat_get(projected, 1, 4));
    }

    freemat(translated);
    freemat(projection_matrix);
    return projected;
}

/**
 * Extracts the screen coordinate from the provided Matrix.
 */
struct fvector2 mat_to_fvector(struct Mat* vec) {
    float x = mat_get(vec, 1, 1);
    float y = mat_get(vec, 1, 2);

    return (struct fvector2) { x, y };
}

/**
 * Returns a rotation matrix with the specified X, Y, and Z rotations applied.
 * Applies in XYZ order.
 */
struct Mat* get_rotation_matrix(float x, float y) {
    struct Mat* x_rot = zeros(3, 3);
    mat_set(x_rot, 1, 1, 1);
    mat_set(x_rot, 2, 2, cos(x));	// [  1    0       0    ]
    mat_set(x_rot, 2, 3, -sin(x));	// [  0  cos(x) -sin(x) ]
    mat_set(x_rot, 3, 2, sin(x));	// [  0  sin(x)  cos(x) ]
    mat_set(x_rot, 3, 3, cos(x));

    struct Mat* y_rot = zeros(3, 3);
    mat_set(y_rot, 1, 1, cos(y));
    mat_set(y_rot, 1, 3, sin(y));	// [  cos(y)  0  sin(y) ]
    mat_set(y_rot, 2, 2, 1);		// [   0      1    0    ]
    mat_set(y_rot, 3, 1, -sin(y));	// [ -sin(y)  0  cos(y) ]
    mat_set(y_rot, 3, 3, cos(y));

    struct Mat* result = multiply(x_rot, y_rot);

    free(x_rot);
    free(y_rot);

    return result;
}
