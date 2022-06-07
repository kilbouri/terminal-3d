#include "TermDebug.h"
#include "TermEngine.h"

#include <stdio.h>

void PrintVector3(Vector3 val);

void DumpMesh(Mesh* mesh) {
    printf("Number of triangles: %d\n", mesh->triangleCount);
    printf("Triangles:");
    for (int i = 0; i < mesh->triangleCount; i++) {
        printf("\n[%d]:\tNormal   = ", i);
        PrintVector3(mesh->triangles[i].normal);
        printf("\n\tVertex 1 = ");
        PrintVector3(mesh->triangles[i].v1);
        printf("\n\tVertex 2 = ");
        PrintVector3(mesh->triangles[i].v2);
        printf("\n\tVertex 3 = ");
        PrintVector3(mesh->triangles[i].v3);
    }
    puts("");
}

void PrintVector3(Vector3 val) {
    printf("[ %lf, %lf, %lf ]", val.x, val.y, val.z);
}
