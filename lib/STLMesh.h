#ifndef matrix_defined
#include "MatLib.h"
#endif

typedef struct face { struct Mat* norm; struct Mat* vert1; struct Mat* vert2; struct Mat* vert3; } face;
typedef struct mesh { char* header; int tri_count; face* triangles; } mesh;

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
	int* t_count = (int*)calloc(1, sizeof(int));
	fread(t_count, 4, 1, source);

	face tris[*t_count];
	for (int i = 0; i < *t_count; i++) {
		struct Mat* norm = ones(1, 4);
		fread(norm->entries, 4, 3, source);
		struct Mat* vert1 = ones(1, 4);
		fread(vert1->entries, 4, 3, source);
		struct Mat* vert2 = ones(1, 4);
		fread(vert2->entries, 4, 3, source);
		struct Mat* vert3 = ones(1, 4);
		fread(vert3->entries, 4, 3, source);

		fseek(source, 2, SEEK_CUR);

		tris[i] = (face){ norm, vert1, vert2, vert3 };
	}

	return (mesh) { hdr, * t_count, tris };
}