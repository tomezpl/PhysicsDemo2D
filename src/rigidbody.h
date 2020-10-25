#pragma once

#include "rectcollider.h"
#include "physics.h"

class RBDynamic
{
public:
	RBDynamic(Point2d Position = Point2d(), float Mass = 1.f, Collider* Collider = new RectCollider())
	{
		position = Position;
		mass = Mass;
		velocity = Vec2();
		collider = Collider;
		overlap = false;
	}

	// Centre of mass position
	Point2d position;
	float mass;
	Vec2 velocity;
	Collider* collider;
	bool overlap;

protected:
	void fall(float time)
	{
		velocity += Vec2(.0f, GRAVITY) * mass;

		position += velocity * time;

		RectCollider* rectCollider = static_cast<RectCollider*>(collider);
		rectCollider->bounds.top += velocity.y * time;
		rectCollider->bounds.bottom += velocity.y * time;
		rectCollider->bounds.left += velocity.x * time;
		rectCollider->bounds.right += velocity.x * time;
	}

	void resolveCollision(Collider& other)
	{
		Point2d closestPoint = other.getClosestPoint(position);
		Vec2 displacement = closestPoint - position;
		Vec2 normV = velocity.normalised();
		displacement -= Vec2(normV.x * collider->getHalfExtents().x, normV.y * collider->getHalfExtents().y);
		position += displacement;
	}

public:
	void step(float time, Collider& other)
	{
		// Apply gravity force.
		fall(time);

		if (collider->isColliding(&other))
		{
			velocity = Vec2();
			resolveCollision(other);
			overlap = true;
		}
		else
		{
			overlap = false;
		}
	}
};