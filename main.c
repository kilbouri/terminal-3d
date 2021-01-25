#include "./lib/MatLib.h"
#include "./lib/STLMesh.h"
#include "./lib/TerminalGraphics.h"

struct Mat* project_screen(struct Mat*);
struct fvector2 mat_to_fvector(struct Mat*);


/**
 * For now, these definitions control parameters
 * used in the rendering pipeline.
 */
#define fNEAR 0.1
#define fFAR 1000
#define FRUSTRUM (fFAR / (fFAR - fNEAR))
#define FOV 90
#define FOV_ADJUSTMENT ((float) 1 / tan(0.5 * FOV * (180.0 / M_PI)))

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

	struct init_bufs buffers = init();
	char* sbuf = buffers.screen_buf;
	float* zbuf = buffers.z_buf;

	/**
	 * Render loop
	 */
	struct vector2 topleft = { 0, 0 };
	struct vector2 bottomright = { get_console_width(), get_console_height() };

	while (1) {
		// reset buffer
		fill_rect(sbuf, topleft, bottomright, 0);

		for (int i = 0; i < model.tri_count; i++) {
			struct Mat* a = project_screen(model.triangles[i].vert1);
			struct Mat* b = project_screen(model.triangles[i].vert2);
			struct Mat* c = project_screen(model.triangles[i].vert3);

			struct vector2 vec_a = norm_to_screen(mat_to_fvector(a));
			struct vector2 vec_b = norm_to_screen(mat_to_fvector(b));
			struct vector2 vec_c = norm_to_screen(mat_to_fvector(c));

			draw(sbuf, vec_a, 1);
			draw(sbuf, vec_b, 1);
			draw(sbuf, vec_c, 1);

			// draw_tri(sbuf, vec_a, vec_b, vec_c, 5);

			freemat(a);
			freemat(b);
			freemat(c);
		}
		show(sbuf);
	}
}



/**
 * Applies the projection matrix to the provided coordinate.
 */
struct Mat* project_screen(struct Mat* coordinate) {
	struct Mat* projection_matrix = zeros(4, 4);
	mat_set(projection_matrix, 1, 1, FOV_ADJUSTMENT * get_aspect());
	mat_set(projection_matrix, 2, 2, FOV_ADJUSTMENT);
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
	float y = mat_get(vec, 2, 1);

	return (struct fvector2){x, y};
}