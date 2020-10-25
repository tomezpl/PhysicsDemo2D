#pragma once

#include "vec.h"

class Rect
{
public:
	// Vertical coordinates for top and bottom edges, and horizontal coordinates for left and right edges.
	float top, left, bottom, right;

	Rect(float Top, float Left, float Bottom, float Right) { top = Top; left = Left; bottom = Bottom; right = Right; }
	Rect(Point2d topLeft = Point2d(), Point2d bottomRight = Point2d()) : Rect(topLeft.y, topLeft.x, bottomRight.y, bottomRight.x) { }

	// Is a Point inside the rectangle's bounds?
	bool inBounds(const Point2d& point)
	{
		return point.y >= top && point.x >= left && point.y <= bottom && point.x <= right;
	}

	inline Point2d topLeft() { return Point2d(left, top); }
	inline Point2d topRight() { return Point2d(right, top); }
	inline Point2d bottomLeft() { return Point2d(left, bottom); }
	inline Point2d bottomRight() { return Point2d(right, bottom); }

	// Is a Rect inside this Rect's bounds?
	bool inBounds(Rect& rect)
	{
		return inBounds(rect.topLeft()) || inBounds(rect.topRight()) || inBounds(rect.bottomLeft()) || inBounds(rect.bottomRight());
	}
};