#pragma once
#include <cmath> // For std::sin, std::cos

#include "Matrix4.h"
#include "Vector4.h"

// Helper function to compare floats with a tolerance
bool AreFloatsEqual(float a, float b, float epsilon = 0.0001f) {
    return fabs(a - b) < epsilon;
}

bool UnitTest() {
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

    printf("Unit Test Passed!");
    return true;
}