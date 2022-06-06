#ifndef STL_MESH_H
#define STL_MESH_H

#include <stdio.h>
#include "../TermEngine.h"

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
Mesh* LoadMesh(FILE* source);

#endif // STL_MESH_H
