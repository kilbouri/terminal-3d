#include <stdio.h>
#include <stdlib.h>

#include "../TermEngine.h"

Vector3 ReadVector3(FILE* source) {
    Vector3 vec;

    fread(&vec.x, 4, 1, source);
    fread(&vec.y, 4, 1, source);
    fread(&vec.z, 4, 1, source);

    return vec;
}

Mesh* LoadMesh(FILE* source) {
    // skip header
    fseek(source, 80, SEEK_CUR);

    // read face count
    int triCount;
    fread(&triCount, 4, 1, source);

    // load triangles
    Mesh* mesh = (Mesh*) malloc(sizeof(Mesh) + sizeof(Triangle) * triCount);
    for (int tri = 0; tri < triCount; tri++) {
        mesh->triangles[tri].normal = ReadVector3(source);
        mesh->triangles[tri].v1 = ReadVector3(source);
        mesh->triangles[tri].v2 = ReadVector3(source);
        mesh->triangles[tri].v3 = ReadVector3(source);

        // skip attribute byte count
        fseek(source, 2, SEEK_CUR);
    }

    return mesh;
}

void FreeMesh(Mesh* mesh) {
    free(mesh);
}
