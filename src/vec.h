#pragma once

#include <cmath>

class Vec2
{
public:
	double x, y;
	Vec2(double X = .0, double Y = .0) { x = X; y = Y; }

	Vec2 operator+(Vec2 rh) { return Vec2(x + rh.x, y + rh.y); }
	Vec2 operator-() { return Vec2(-x, -y); }
	Vec2 operator-(Vec2 rh) { return Vec2(x - rh.x, y - rh.y); }
	Vec2 operator/(float rh) { return rh == .0f ? Vec2() : Vec2(x / rh, y / rh); }
	Vec2 operator*(float rh) { return Vec2(x * rh, y * rh); }
	void operator+=(Vec2 rh) { *this = *this + rh; }
	void operator-=(Vec2 rh) { *this += -rh; }
	void operator*=(float rh) { *this = *this * rh; }
	void operator/=(float rh) { *this = *this / rh; }

	static float dot(Vec2 lh, Vec2 rh) { return lh.x * rh.x + lh.y * rh.y; }

	float magnitude() { return sqrtf(x * x + y * y); }
	Vec2 normalised() { return *this / magnitude(); }
};

typedef Vec2 Point2d;