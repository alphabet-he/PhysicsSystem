#pragma once

class Point2D
{

public:
	
	int x;
	int y;

	Point2D(int _x, int _y);

	void setX(int _x) { x = _x; }
	void setY(int _y) { y = _y; }

	Point2D operator+(Point2D p2) {
		return Point2D(x + p2.x, y + p2.y);
	}

	Point2D operator-(Point2D p2) {
		return Point2D(x - p2.x, y - p2.y);
	}

	Point2D operator*(int i) {
		return Point2D(x * i, y * i);
	}

	Point2D operator/(int i) {
		return Point2D(x / i, y / i);
	}

	Point2D operator-() {
		return Point2D(-x, -y);
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

	Point2D operator*=(int i) {
		x *= i;
		y *= i;
		return *this;
	}

	Point2D operator/=(int i) {
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

};

inline Point2D operator* (int i, const Point2D& p)
{
	return Point2D(p.x * i, p.y * i);
}

//Point2D operator+(Point2D p1, Point2D p2) {
//	return Point2D(p1.x + p2.x, p1.y + p2.y);
//}

//Point2D operator+=(Point2D self, Point2D other) {
//	self.x += other.x;
//	self.y += other.y;
//	return self;
//}



