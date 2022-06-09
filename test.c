#include <stdio.h>
#include <assert.h>

#include "src/TermEngine/Math/Vector.h"
#include "src/TermEngine/Math/Matrix.h"
#include "src/TermEngine/Math/Quaternion.h"

#define Abs(x) ((x) >= 0 ? x : -x)

int Approx(float x, float y) {
    return Abs(x - y) <= 0.0001f;
}

int QuaternionEqual(Quaternion a, Quaternion b) {
    return Approx(a.x, b.x) && Approx(a.y, b.y) && Approx(a.z, b.z) && Approx(a.w, b.w);
}

void PrintQuaternion(Quaternion a) {
    printf("{ x = %lf, y = %lf, z = %lf, w = %lf }\n", a.x, a.y, a.z, a.w);
}

int main(void) {

    Vector3 EulerA = {30, 0, 0};
    Vector3 EulerB = {10, 10, 5};

    Quaternion QuatA = FromEuler(EulerA);
    Quaternion QuatB = FromEuler(EulerB);

    Quaternion CorrectQuatA = {0.258819f, 0, 0, 0.9659258f};
    Quaternion CorrectQuatB = {0.0905287f, 0.0829542f, 0.0508769f, 0.991128f};

    PrintQuaternion(QuatA);
    PrintQuaternion(QuatB);
    assert(QuaternionEqual(QuatA, CorrectQuatA));
    assert(QuaternionEqual(QuatB, CorrectQuatB));

    Quaternion QuatAB = MulQuaternion(QuatA, QuatB);
    Quaternion CorrectQuatAB = {-0.16908f, 0.06696f, 0.07061f, 0.98079f};

    PrintQuaternion(QuatAB);
    assert(QuaternionEqual(QuatAB, CorrectQuatAB));
}
