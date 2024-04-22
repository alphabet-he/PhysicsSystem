#include "Vector4.h"
#include "Matrix4.h"

Vector4 Vector4::operator*(const Matrix4& matrix) const
{
    return Vector4(
        x * matrix.m[0][0] + y * matrix.m[0][1] + z * matrix.m[0][2] + w * matrix.m[0][3],
        x * matrix.m[1][0] + y * matrix.m[1][1] + z * matrix.m[1][2] + w * matrix.m[1][3],
        x * matrix.m[2][0] + y * matrix.m[2][1] + z * matrix.m[2][2] + w * matrix.m[2][3],
        x * matrix.m[3][0] + y * matrix.m[3][1] + z * matrix.m[3][2] + w * matrix.m[3][3]
    );
}

Vector4 Vector4::operator*=(const Matrix4& matrix)
{
    // First, calculate the new values
    float newX = x * matrix.m[0][0] + y * matrix.m[0][1] + z * matrix.m[0][2] + w * matrix.m[0][3];
    float newY = x * matrix.m[1][0] + y * matrix.m[1][1] + z * matrix.m[1][2] + w * matrix.m[1][3];
    float newZ = x * matrix.m[2][0] + y * matrix.m[2][1] + z * matrix.m[2][2] + w * matrix.m[2][3];
    float newW = x * matrix.m[3][0] + y * matrix.m[3][1] + z * matrix.m[3][2] + w * matrix.m[3][3];

    // Update the vector's own components
    x = newX;
    y = newY;
    z = newZ;
    w = newW;

    // Return a reference to this object
    return *this;
}
