#include <stdio.h>
#include <stdlib.h>

#include "t3dmath.h"

/**
 * Loads a non-ASCII STL Mesh by the standard:
 *
 * char[80]  – Header                 - 80 bytes
 * int       – Number of triangles    -  4 bytes
 *
 * foreach triangle                   - 50 bytes:
 * 	float[3] – Normal vector          - 12 bytes
 * 	float[3] – Vertex 1               - 12 bytes
 * 	float[3] – Vertex 2               - 12 bytes
 * 	float[3] – Vertex 3               - 12 bytes
 * 	short    – Attribute byte count   -  2 bytes (ignored)
 * end
 */
Mesh load_stl(FILE* source) {
	// skip header
	fseek(source, 80, SEEK_CUR);

	// read face count
	int triCount;
	fread(&triCount, 4, 1, source);

	// load triangles
	Triangle* triangles = (Triangle*) malloc(sizeof(Triangle) * triCount);

	for (int tri = 0; tri < triCount; tri++) {
		// read 4 float3s
		float x, y, z;
		fread(&x, 4, 1, source);
		fread(&y, 4, 1, source);
		fread(&z, 4, 1, source);
		triangles[tri].normal = (Float3) { x, y, z };

		fread(&x, 4, 1, source);
		fread(&y, 4, 1, source);
		fread(&z, 4, 1, source);
		triangles[tri].v1 = (Float3) { x, y, z };

		fread(&x, 4, 1, source);
		fread(&y, 4, 1, source);
		fread(&z, 4, 1, source);
		triangles[tri].v2 = (Float3) { x, y, z };

		fread(&x, 4, 1, source);
		fread(&y, 4, 1, source);
		fread(&z, 4, 1, source);
		triangles[tri].v3 = (Float3) { x, y, z };

		// skip triangle byte count
		fseek(source, 2, SEEK_CUR);
	}

	return (Mesh) { triCount, triangles };
}