#pragma once
#include <cmath>
#include "Point2D.h"

struct Vector3
{

	float x;
	float y;
	float z;

	Vector3() :Vector3(0.0f, 0.0f, 0.0f) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const Point2D& p, float i_z) : x(p.x), y(p.y), z(i_z) {}

	Vector3 operator+(Vector3 p2) {
		return Vector3{ x + p2.x, y + p2.y, z + p2.z };
	}

	Vector3 operator-(Vector3 p2) {
		return Vector3{ x - p2.x, y - p2.y, z - p2.z };
	}

	Vector3 operator*(float i) {
		return Vector3{ x * i, y * i, z * i };
	}

	Vector3 operator/(float i) {
		return Vector3{ x / i, y / i, z / i };
	}

	Vector3 operator-() {
		return Vector3{ -x, -y, -z };
	}

	Vector3 operator+=(Vector3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 operator-=(Vector3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3 operator*=(float i) {
		x *= i;
		y *= i;
		z *= i;
		return *this;
	}

	Vector3 operator/=(float i) {
		x /= i;
		y /= i;
		z /= i;
		return *this;
	}

	bool operator==(Vector3 other) {
		if (x == other.x && y == other.y && z == other.z) {
			return true;
		}
		return false;
	}

	bool operator!=(Vector3 other) {
		if (x != other.x || y != other.y || z != other.z) {
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
		}
	}

	Vector3 Normalized() const {
		float len = Length();
		if (len != 0) {
			return Vector3(x / len, y / len, z / len);
		}
		return *this; // Return the original vector if it's zero-length
	}

	float Length() const {
		return std::sqrt(LengthSq());
	}


	float LengthSq() const {
		return (x * x + y * y + z * z);
	}

	static const Vector3 Zero;
};

