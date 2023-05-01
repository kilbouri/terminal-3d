#include <stdlib.h>

#include "../Engine.h"

Vector3 ReadVector3(FILE* source) {
    float tempX, tempY, tempZ;

    fread(&tempX, 4, 1, source);
    fread(&tempY, 4, 1, source);
    fread(&tempZ, 4, 1, source);

    return (Vector3) {tempX, tempY, tempZ};
}

Mesh* LoadMeshFromSTL(FILE* stlSource) {
    if (stlSource == NULL) {
        return NULL;
    }

    // skip header
    fseek(stlSource, 80, SEEK_CUR);

    // read triangle count
    int triCount;
    fread(&triCount, 4, 1, stlSource);

    // load triangles
    Mesh* mesh = AllocateMesh(triCount);
    for (int tri = 0; tri < triCount; tri++) {
        mesh->triangles[tri].normal = ReadVector3(stlSource);
        mesh->triangles[tri].v1 = ReadVector3(stlSource);
        mesh->triangles[tri].v2 = ReadVector3(stlSource);
        mesh->triangles[tri].v3 = ReadVector3(stlSource);

        // skip attribute byte count
        fseek(stlSource, 2, SEEK_CUR);
    }

    return mesh;
}

Mesh* AllocateMesh(int triangleCount) {
    Mesh* mesh = malloc(sizeof(Mesh) + sizeof(Triangle) * triangleCount);
    mesh->triangleCount = triangleCount;

    return mesh;
}

void FreeMesh(Mesh* mesh) {
    free(mesh);
}
