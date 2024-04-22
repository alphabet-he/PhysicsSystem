#pragma once
#include <cmath>
#include "Vector3.h"


struct Matrix4;  // Forward declaration

struct Vector4
{

	float x;
	float y;
	float z;
	float w;

	Vector4() :Vector4(0.0f, 0.0f, 0.0f, 0.0f) {};
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(const Point2D& p, float i_z, float i_w) : x(p.x), y(p.y), z(i_z), w(i_w) {}
	Vector4(const Vector3& p, float i_w) : x(p.x), y(p.y), z(p.z), w(i_w) {}

	Vector4 operator+(Vector4 p2) {
		return Vector4{ x + p2.x, y + p2.y, z + p2.z, w + p2.w };
	}

	Vector4 operator-(Vector4 p2) {
		return Vector4{ x - p2.x, y - p2.y, z - p2.z, w - p2.w };
	}

	Vector4 operator*(float i) {
		return Vector4{ x * i, y * i, z * i, w * i };
	}

	Vector4 operator*(const Matrix4& matrix) const;

	Vector4 operator/(float i) {
		return Vector4{ x / i, y / i, z / i, w / i };
	}

	Vector4 operator-() {
		return Vector4{ -x, -y, -z, -w };
	}

	Vector4 operator+=(Vector4 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vector4 operator-=(Vector4 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	Vector4 operator*=(float i) {
		x *= i;
		y *= i;
		z *= i;
		w *= i;
		return *this;
	}

	Vector4 operator*=(const Matrix4& matrix);

	Vector4 operator/=(float i) {
		x /= i;
		y /= i;
		z /= i;
		w /= i;
		return *this;
	}

	bool operator==(Vector4 other) {
		if (x == other.x && y == other.y && z == other.z && w == other.w) {
			return true;
		}
		return false;
	}

	bool operator!=(Vector4 other) {
		if (x != other.x || y != other.y || z != other.z || w != other.w) {
			return true;
		}
		return false;
	}

	void Normalize() {
		float len = Length();
		if (len != 0) {
			x /= len;
			y /= len;
			z /= len;
			w /= len;
		}
	}

	Vector4 Normalized() const {
		float len = Length();
		if (len != 0) {
			return Vector4(x / len, y / len, z / len, w / len);
		}
		return *this; // Return the original vector if it's zero-length
	}

	float Length() const {
		return std::sqrt(LengthSq());
	}


	float LengthSq() const {
		return (x * x + y * y + z * z + w * w);
	}

	static const Vector4 Zero;
};

