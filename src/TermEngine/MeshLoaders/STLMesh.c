#include <stdio.h>
#include <stdlib.h>

#include "../TermEngine.h"

Vector3 ReadVector3(FILE* source) {
    float tempX, tempY, tempZ;

    fread(&tempX, 4, 1, source);
    fread(&tempY, 4, 1, source);
    fread(&tempZ, 4, 1, source);

    return (Vector3) { tempX, tempY, tempZ };
}


Mesh* LoadMesh(FILE* source) {
    // skip header
    fseek(source, 80, SEEK_CUR);

    // read face count
    int triCount;
    fread(&triCount, 4, 1, source);

    // load triangles
    Mesh* mesh = (Mesh*) malloc(sizeof(Mesh) + sizeof(Triangle) * triCount);
    mesh->triangleCount = triCount;

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
