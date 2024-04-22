#pragma once
#include <cmath> // For std::sin, std::cos

#include "Matrix4.h"
#include "Vector4.h"

// Helper function to compare floats with a tolerance
bool AreFloatsEqual(float a, float b, float epsilon = 0.0001f) {
    return fabs(a - b) < epsilon;
}

bool UnitTest() {

    float pi = 3.1415926535897f;

    // test vector4

    // equal
    Vector4 a(1, 5, 7, 9);
    Vector4 b(8, 10, 21, 64);
    assert(a != b);

    Vector4 c(0, 0, 0, 0);
    c = a;
    assert(c == a);

    // add
    c = Vector4(9, 15, 28, 73);
    assert(a + b == c);

    // minus
    c = Vector4(7, 5, 14, 55);
    assert(b - a == c);

    // multiply
    float i = 6;
    c = Vector4(6, 30, 42, 54);
    assert(a * i == c);
    a *= i;
    assert(a == c);

    // divided
    i = 2;
    c = Vector4(4, 5, 10.5, 32);
    assert(b / i == c);
    b /= i;
    assert(b == c);

    // Length
    Vector4 d = Vector4(2, 4, 5, -6);
    assert(d.Length() == 9);

    // Normalize
    Vector4 normalized = Vector4(2.0f / 9.0f, 4.0f / 9.0f, 5.0f / 9.0f, -2.0f / 3.0f);
    assert(d.Normalized() == normalized);
    d.Normalize();
    assert(d == normalized);


    // matrix 4*4

    // equal
    Matrix4 q = Matrix4(
        7, 3, 5, 6,
        2, 5, 8, -3,
        -5, 4, 1, 2,
        8, 12, -9, 23
    );
    Matrix4 w = Matrix4(
        1, 0, 7, 2,
        9, 4, 3, -5,
        -6, 1, 2, 3,
        5, -8, -7, 4
    );
    assert(q != w);

    // multiply
    Matrix4 e = Matrix4(
        34, -31, 26, 38,
        -16, 52, 66, -9,
        35,	1, -35, -19,
        285, -145, -87,	21
    );
    assert(q * w == e);

    // M * V
    Vector4 v = Vector4(1, 3, 7, 4);
    c = Vector4(75, 61, 22, 73);
    assert(q * v == c);

    // V * M
    c = Vector4(10, 94, 0, 103);
    v *= q;
    assert(v == c);

    // row
    c = Vector4(2, 5, 8, -3);
    assert(q.Row(1) == c);

    // column
    c = Vector4(5, 8, 1, -9);
    assert(q.Col(2) == c);

    // translation matrix
    Vector3 p = Vector3(1, 3, 7);
    Vector3 transV = Vector3(2, 4, 2);
    Matrix4 transM = Matrix4::CreateTranslationCV(transV);
    Vector3 target = Vector3(3, 7, 9);
    assert(transM.TransformPoint(p) == target);

    // scale matrix
    transM = Matrix4::CreateScale(1, 2, 2);
    target = Vector3(1, 6, 14);
    assert(transM.TransformPoint(p) == target);

    // rotation matrix
    // x
    transM = Matrix4::CreateXRotationCV(pi / 2.0f);
    target = Vector3(1, -7, 3);
    assert(AreFloatsEqual(transM.TransformPoint(p).x, target.x));
    assert(AreFloatsEqual(transM.TransformPoint(p).y, target.y));
    assert(AreFloatsEqual(transM.TransformPoint(p).z, target.z));
    // y
    transM = Matrix4::CreateYRotationCV(pi / 2.0f);
    target = Vector3(7, 3, -1);
    assert(AreFloatsEqual(transM.TransformPoint(p).x, target.x));
    assert(AreFloatsEqual(transM.TransformPoint(p).y, target.y));
    assert(AreFloatsEqual(transM.TransformPoint(p).z, target.z));

    // z
    transM = Matrix4::CreateZRotationCV(pi / 2.0f);
    target = Vector3(-3, 1, 7);
    assert(AreFloatsEqual(transM.TransformPoint(p).x, target.x));
    assert(AreFloatsEqual(transM.TransformPoint(p).y, target.y));
    assert(AreFloatsEqual(transM.TransformPoint(p).z, target.z));

    // invert
    transM.Invert();
    assert(AreFloatsEqual(transM.TransformPoint(target).x, p.x));
    assert(AreFloatsEqual(transM.TransformPoint(target).y, p.y));
    assert(AreFloatsEqual(transM.TransformPoint(target).z, p.z));

    // invert with rotation and translation
    Matrix4 transRot = Matrix4::CreateZRotationCV(pi / 2.0f);
    Matrix4 transLation = Matrix4::CreateTranslationCV(2, 5, 7);
    transM =  transLation * transRot;
    target = Vector3(-1, 6, 14);
    assert(AreFloatsEqual(transM.TransformPoint(p).x, target.x));
    assert(AreFloatsEqual(transM.TransformPoint(p).y, target.y));
    assert(AreFloatsEqual(transM.TransformPoint(p).z, target.z));

    Matrix4 inverted = transM.GetInverseRotTransCV();
    assert(AreFloatsEqual(inverted.TransformPoint(target).x, p.x));
    assert(AreFloatsEqual(inverted.TransformPoint(target).y, p.y));
    assert(AreFloatsEqual(inverted.TransformPoint(target).z, p.z));

    return true;
}

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const Matrix4 Matrix4::Identity(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);