#ifndef matrix_defined
#include "MatLib.h"
#endif

/**
 * Stores a mesh in a 4x4 matrix:
 * [ normal ]
 * [ vert 1 ]
 * [ vert 2 ]
 * [ vert 3 ]
 */
typedef struct mesh { char* header; int tri_count; struct Mat** triangles; } mesh;

/**
 * Loads a non-ASCII STL mesh by the standard:
 *
 * char[80]    – Header               -     80 bytes
 * int         – Number of triangles  -      4 bytes
 *
 * foreach triangle                   - 50 bytes:
 * 	float[3] – Normal vector          - 12 bytes
 * 	float[3] – Vertex 1               - 12 bytes
 * 	float[3] – Vertex 2               - 12 bytes
 * 	float[3] – Vertex 3               - 12 bytes
 * 	short    – Attribute byte count   -  2 bytes (ignored)
 * end
 */
mesh load_stl(FILE* source) {
	// read header
	char* hdr = (char*)calloc(81, sizeof(char));
	fread(hdr, 80, 1, source);

	// read face count
	int t_count;
	fread(&t_count, 4, 1, source);

	struct Mat** tris = (struct Mat**)calloc(t_count, sizeof(struct Mat));
	for (int i = 0; i < t_count; i++) {
		tris[i] = ones(4, 4);
		for (int r = 1; r <= 4; r++) {
			for (int c = 1; c < 4; c++) {
				float value;
				fread(&value, 4, 1, source);
				
				mat_set(tris[i], r, c, value);
			}
		}

		fseek(source, 2, SEEK_CUR);
	}
	return (mesh) { hdr, t_count, tris };
}