#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y, z, w;
} Vector4;

#define ZerosVector3 \
    { 0, 0, 0 }
#define OnesVector3 \
    { 1, 1, 1 }
#define ZerosVector4 \
    { 0, 0, 0, 0 }
#define OnesVector4 \
    { 1, 1, 1, 1 }

Vector3 AddVector3(Vector3 a, Vector3 b);
Vector3 SubVector3(Vector3 a, Vector3 b);
Vector3 MulVector3(Vector3 vec, float scalar);
Vector3 CrossVector3(Vector3 a, Vector3 b);
Vector3 NormalizeVector3(Vector3 vec);
float DotVector3(Vector3 a, Vector3 b);

Vector4 AddVector4(Vector4 a, Vector4 b);
Vector4 SubVector4(Vector4 a, Vector4 b);
Vector4 MulVector4(Vector4 vec, float scalar);
Vector4 NormalizeVector4(Vector4 vec);
float DotVector4(Vector4 a, Vector4 b);

#endif // VECTOR_H
