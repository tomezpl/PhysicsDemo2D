#pragma once

#include "rect.h"
#include "collider.h"

class RectCollider : public Collider
{
public:
	Rect bounds;
	RectCollider(Rect Bounds = Rect()) { bounds = Bounds; }

	Point2d getHalfExtents()
	{
		return Point2d(((bounds.left + bounds.right) / 2.f), ((bounds.top + bounds.bottom) / 2.f));
	}

	Point2d getClosestPoint(Point2d& position)
	{
		// Compute horizontal and vertical projections.
		Vec2 hDir = bounds.topRight() - bounds.topLeft();
		Vec2 vDir = bounds.bottomLeft() - bounds.topLeft();

		// Displacement of the position relative to this collider's frame.
		Vec2 offset = position - bounds.topLeft();

		// Compute lengths of each projection.
		float projH = Vec2::dot(hDir.normalised(), offset.normalised());
		float projV = Vec2::dot(vDir.normalised(), offset.normalised());

		// Find exact projected points along each edge using the projection lengths.
		Point2d projections[4] = 
		{ 
			bounds.topLeft() + hDir * projH, 
			bounds.topLeft() + vDir * projV, 
			bounds.topRight() + vDir * projV, 
			bounds.bottomLeft() + hDir * projH 
		};

		Point2d closest;
		
		for (int i = 0; i < 4; i++)
		{
			bool isShortest = i == 0 || ((position - closest).magnitude() < (position - projections[i]).magnitude());
			if (isShortest)
			{
				closest = projections[i];
			}
		}

		return closest;
	}

	bool isColliding(Collider* collider)
	{
		RectCollider rectCollider = *reinterpret_cast<RectCollider*>(collider);
		return rectCollider.bounds.inBounds(bounds);
	}
};
