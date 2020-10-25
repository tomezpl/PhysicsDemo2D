#pragma once

#include "vec.h"

class Collider {
public:
	// Returns a point on the collider's surface that's the closest to the specified point.
	virtual Point2d getClosestPoint(Point2d& position) = 0;

	virtual Point2d getHalfExtents() = 0;

	virtual bool isColliding(Collider* collider) = 0;
};