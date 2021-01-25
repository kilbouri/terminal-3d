#include "./lib/MatLib.h"
#include "./lib/STLMesh.h"
#include "./lib/TerminalGraphics.h"
#include "./lib/Utilities.h"

struct Mat* project_screen(struct Mat*);
struct fvector2 mat_to_fvector(struct Mat*);
struct Mat* get_rotation_matrix(float, float);

/**
 * For now, these definitions control parameters
 * used in the rendering pipeline.
 * 
 * fNEAR 				- near clipping plane
 * fFAR 				- far clipping plane
 * FRUSTRUM 			- the camera view plane calculation
 * FOV 					- the camera's field of view. Integer multiples of 180 are bad news!
 * DIST_CORRECTION 		- the distance correction calculation
 * TERMINAL_CORRECTION	- scales the vertical axis to fix vertical stretching in the final rasterization
 * [XY]_ROT_SPEED		- how fast should the mesh rotate? (measured in radians/frame) TODO: Normalize framerate to max of 30fps
 * SCALE				- scales the model evenly along all axes
 */
#define fNEAR 0.1
#define fFAR 1000
#define FRUSTRUM (fFAR / (fFAR - fNEAR))
#define FOV 135
#define DIST_CORRECTION (1.0 / tan(FOV * 0.5 * (3.1415926 / 180.0)))
#define TERMINAL_CORRECTION 0.5
#define X_ROT_SPEED 0.010;
#define Y_ROT_SPEED 0.015;

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

	struct init_bufs buffers = engine_init();
	char* sbuf = buffers.screen_buf;
	float* zbuf = buffers.z_buf;

	/**
	 * Render loop
	 */
	struct vector2 topleft = { 0, 0 };
	struct vector2 bottomright = { get_console_width(), get_console_height() };

	float x_rot = 0;
	float y_rot = 0;

	while (1) {
		// reset buffer (keep as close to computation loop as possible to minimize flashing!)
		fill_rect(sbuf, topleft, bottomright, 0);

		// get this frame's rotation matrix
		struct Mat* rotation = get_rotation_matrix(x_rot, y_rot);
		for (int i = 0; i < model.tri_count; i++) {

			struct Mat* rotated_a = multiply(model.triangles[i].vert1, rotation);
			struct Mat* rotated_b = multiply(model.triangles[i].vert2, rotation);
			struct Mat* rotated_c = multiply(model.triangles[i].vert3, rotation);

			struct Mat* a = project_screen(rotated_a);
			struct Mat* b = project_screen(rotated_b);
			struct Mat* c = project_screen(rotated_c);

			struct vector2 vec_a = norm_to_screen(mat_to_fvector(a));
			struct vector2 vec_b = norm_to_screen(mat_to_fvector(b));
			struct vector2 vec_c = norm_to_screen(mat_to_fvector(c));

			// draw calls
			draw_tri(sbuf, vec_a, vec_b, vec_c, 1);

			// garbage collection
			freemat(rotated_a);
			freemat(rotated_b);
			freemat(rotated_c);

			freemat(a);
			freemat(b);
			freemat(c);
		}

		// display
		show(sbuf);

		x_rot += X_ROT_SPEED;
		y_rot += Y_ROT_SPEED;

		freemat(rotation);
	}
}



/**
 * Applies the projection matrix to the provided coordinate.
 */
struct Mat* project_screen(struct Mat* coordinate) {
	struct Mat* projection_matrix = zeros(4, 4);

	double wtf = DIST_CORRECTION;

	mat_set(projection_matrix, 1, 1, DIST_CORRECTION * get_aspect());
	mat_set(projection_matrix, 2, 2, DIST_CORRECTION * TERMINAL_CORRECTION);
	mat_set(projection_matrix, 3, 3, FRUSTRUM);
	mat_set(projection_matrix, 4, 3, -fNEAR * FRUSTRUM);
	mat_set(projection_matrix, 3, 4, 1);

	struct Mat* translated = scalermultiply(coordinate, 1);
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

	return (struct fvector2){x, y};
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