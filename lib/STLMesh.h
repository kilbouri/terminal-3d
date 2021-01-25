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
	int t_count;
	fread(&t_count, 4, 1, source);

	face* tris = (face*)calloc(t_count, sizeof(face));
	for (int i = 0; i < t_count; i++) {
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

	return (mesh) { hdr, t_count, tris };
}

void print_mesh(mesh m) {
	printf("Header: %s\n", m.header);
	printf("Triangle Count: %d\n", m.tri_count);

	for (int i = 0; i < m.tri_count; i++) {
		printf("%d: \nNorm: \n", i);
		showmat(m.triangles[i].norm);
		
		printf("Vert1: \n");
		showmat(m.triangles[i].vert1);

		printf("Vert2: \n");
		showmat(m.triangles[i].vert2);

		printf("Vert3: \n");
		showmat(m.triangles[i].vert3);
	}
}