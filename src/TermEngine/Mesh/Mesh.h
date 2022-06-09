#ifndef MESH_H
#define MESH_H

#include <stdio.h>

#include "../Math/Vector.h"

typedef struct {
    Vector3 normal;
    Vector3 v1, v2, v3;
} Triangle;

typedef struct {
    int triangleCount;
    Triangle triangles[];
} Mesh;

/**
 * Loads a non-ASCII STL Mesh by the standard:
 *
 * char[80]    – Header               - 80 bytes (ignored)
 * int         – Number of triangles  -  4 bytes
 *
 * foreach triangle                   - 50 bytes:
 * 	  float[3] – Normal vector        - 12 bytes
 * 	  float[3] – Vertex 1             - 12 bytes
 * 	  float[3] – Vertex 2             - 12 bytes
 * 	  float[3] – Vertex 3             - 12 bytes
 * 	  short    – Attribute byte count -  2 bytes (ignored)
 * end
 */
Mesh* LoadMeshFromSTL(FILE* stlSource);

Mesh* AllocateMesh(int triangleCount);
void FreeMesh(Mesh* mesh);

#endif // MESH_H
