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
		airDragForce = Vec2();
		collider = Collider;
		overlap = false;
	}

	// Centre of mass position
	Point2d position;
	float mass;
	Vec2 velocity, airDragForce;
	Collider* collider;
	bool overlap;

protected:
	RectCollider* getRectCollider() 
	{
		return static_cast<RectCollider*>(collider);
	}

	void fall(float time)
	{
		RectCollider* collider = getRectCollider();
		Vec2 fG = Vec2(.0f, GRAVITY) * mass;
		// Assume a Z dimension.
		const float length = 1.f;

		// Approx. air density.
		const float airDensity = 1.2;

		// TODO: This only works for a square that is falling straight down. Does not account for rotations or other shapes.
		float crossSectionalArea = Vec2(collider->bounds.left - collider->bounds.right).magnitude() * length;

		// Drag coefficient.
		const float cd = 0.01f;

		velocity = Vec2(.0f, sqrtf((2.f * fG.y) / (airDensity * crossSectionalArea * cd)));
	}

	void airDrag()
	{
		// Calculate area (TODO: refactor for other colliders!)
		RectCollider* collider = getRectCollider();

		// Assume a Z dimension.
		const float length = 1.f;

		// TODO: This only works for a square that is falling straight down. Does not account for rotations or other shapes.
		float crossSectionalArea = Vec2(collider->bounds.left - collider->bounds.right).magnitude() * length;

		// Drag coefficient.
		const float cd = 0.7f;

		// Calculate volume
		float volume = Vec2(collider->bounds.left - collider->bounds.right).magnitude() * Vec2(collider->bounds.top - collider->bounds.bottom).magnitude() * length;
		
		// Calculate mass density
		float massDensity = mass / volume;

		// Approx. air density.
		const float airDensity = 1.2;

		// Calculate the drag force
		float fD = .5f * airDensity * crossSectionalArea * cd * powf(velocity.y, 2.f);

		airDragForce = Vec2(0.f, fD);
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
	void step(double time, Collider* other)
	{
		// Apply gravity force.
		if (!overlap) 
		{
			fall(time);
		}

		// Apply air resistance.
		airDrag();

		// Integrate velocity into position over time.
		position += velocity * time;

		// Update the collider.
		// TODO: This needs to be refactored to support other colliders.
		RectCollider* rectCollider = getRectCollider();
		rectCollider->bounds.top += velocity.y * time;
		rectCollider->bounds.bottom += velocity.y * time;
		rectCollider->bounds.left += velocity.x * time;
		rectCollider->bounds.right += velocity.x * time;

		if (other != nullptr && collider->isColliding(other))
		{
			position -= velocity * time;
			velocity = Vec2();
			resolveCollision(*other);
			overlap = true;
		}
		else
		{
			overlap = false;
		}
	}

	void addForce(Vec2 force)
	{
		velocity += force / mass;
	}
};