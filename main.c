#include "./lib/MatLib.h"
#include "./lib/STLMesh.h"
#include "./lib/TerminalGameEngine.h"


struct Mat* project_screen(struct Mat*);
struct Mat* scale_to_screen(struct Mat*);

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

	/**
	 * Render loop
	 */
	while (1) {
		printf("Transforming for new frame...\n");
		for (int i = 0; i < model.tri_count; i++) {
			/**
			 * TODO: This is insanely inefficient. Each frame requires a tonne of garbage cleanup, FIX THAT!
			 * TODO: Fix seg fault when actually iterating over each of the triangles in the mesh, instead of the same one repeatedly.
			 * 		 Possibly out of bounds? Seems rather unlikely but idk its 1:40 and I'm going to bed.
			 */
			struct Mat* projected_vert1 = project_screen(model.triangles[0].vert1);
			struct Mat* projected_vert2 = project_screen(model.triangles[0].vert2);
			struct Mat* projected_vert3 = project_screen(model.triangles[0].vert3);

			struct Mat* scaled_vert1 = scale_to_screen(projected_vert1);
			struct Mat* scaled_vert2 = scale_to_screen(projected_vert2);
			struct Mat* scaled_vert3 = scale_to_screen(projected_vert3);

			printf("Transformations complete. TODO: Draw now!\n");

			printf("End of this frame's, garbage collection time.\n");
			freemat(projected_vert1);
			freemat(projected_vert2);
			freemat(projected_vert3);

			freemat(scaled_vert1);
			freemat(scaled_vert2);
			freemat(scaled_vert3);
		}
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

	struct Mat* projected = multiply(coordinate, projection_matrix);
	
	if (!mat_get(projected, 1, 4)) {
		mat_set(projected, 1, 1, mat_get(projected, 1, 1) / mat_get(projected, 1, 4));
		mat_set(projected, 1, 2, mat_get(projected, 1, 2) / mat_get(projected, 1, 4));
	}

	freemat(translated);
	freemat(projection_matrix);
	return projected;
}

/**
 * Scales a normalized coordinate to screen space
 */
struct Mat* scale_to_screen(struct Mat* proj_coordinate) {
	struct Mat* adjusted = scalermultiply(proj_coordinate, 1);
	mat_set(adjusted, 1, 1, (mat_get(proj_coordinate, 1, 1) + 1) * get_console_width() * 0.5);
	mat_set(adjusted, 1, 2, (mat_get(proj_coordinate, 1, 2) + 1) * get_console_height() * 0.5);

	return adjusted;
}