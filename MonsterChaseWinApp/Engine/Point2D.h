#pragma once
#include <GLib.h>
#include <cmath>

struct Point2D
{

	float x;
	float y;

	Point2D operator+(Point2D p2) {
		return Point2D{ x + p2.x, y + p2.y };
	}

	Point2D operator-(Point2D p2) {
		return Point2D{ x - p2.x, y - p2.y };
	}

	Point2D operator*(float i) {
		return Point2D{ x * i, y * i };
	}

	Point2D operator/(float i) {
		return Point2D{ x / i, y / i };
	}

	Point2D operator-() {
		return Point2D{ -x, -y };
	}

	Point2D operator+=(Point2D other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Point2D operator-=(Point2D other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point2D operator*=(float i) {
		x *= i;
		y *= i;
		return *this;
	}

	Point2D operator/=(float i) {
		x /= i;
		y /= i;
		return *this;
	}

	bool operator==(Point2D other) {
		if (x == other.x && y == other.y) {
			return true;
		}
		return false;
	}

	bool operator!=(Point2D other) {
		if (x == other.x && y == other.y) {
			return false;
		}
		return true;
	}

	GLib::Point2D ToGlibPoint2D() {
		return GLib::Point2D{ x, y };
	}

	static float GetDist(Point2D a, Point2D b) {
		Point2D dist = a - b;
		return dist.Length();
	}

	float Length() {
		return std::sqrt(x * x + y * y);
	}

};

//Point2D operator+(Point2D p1, Point2D p2) {
//	return Point2D(p1.x + p2.x, p1.y + p2.y);
//}

//Point2D operator+=(Point2D self, Point2D other) {
//	self.x += other.x;
//	self.y += other.y;
//	return self;
//}



