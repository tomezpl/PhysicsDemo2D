#pragma once

#include "rectcollider.h"
#include "physics.h"

class RBDynamic
{
	friend class SpringConstraint;
public:
	RBDynamic(Point2d Position = Point2d(), float Mass = 1.f, bool kinematic = false, Collider* Collider = new RectCollider())
	{
		position = Position;
		mass = Mass;
		velocity = Vec2();
		localCollider = Collider;
		collider = new RectCollider();
		overlap = false;
		isKinematic = kinematic;
	}

	// Centre of mass position
	Point2d position;
	float mass;
	Vec2 velocity;
	Collider* collider, *localCollider;
	bool overlap;
	bool isKinematic;

protected:
	RectCollider* getRectCollider() 
	{
		return reinterpret_cast<RectCollider*>(localCollider);
	}

	void fall(float time)
	{
		Vec2 fG = Vec2(.0f, GRAVITY) * mass;

		addForce(fG);
	}

	// Update the world-space collider.
	void updateRectBounds()
	{
		// TODO: This needs to be refactored to support other colliders.
		RectCollider* rectCollider = reinterpret_cast<RectCollider*>(collider);
		Point2d hx = localCollider->getHalfExtents();

		rectCollider->bounds.top = (position.y - hx.y);
		rectCollider->bounds.bottom = (position.y + hx.y);
		rectCollider->bounds.left = (position.x - hx.x);
		rectCollider->bounds.right = (position.x + hx.x);
	}

public:
	Vec2 airDrag()
	{
		// Calculate area (TODO: refactor for other colliders!)
		RectCollider* collider = getRectCollider();

		// Assume a Z dimension.
		const float length = 1.f;

		// TODO: This only works for a square that is falling straight down. Does not account for rotations or other shapes.
		float crossSectionalAreaBottom = Vec2(collider->bounds.left - collider->bounds.right).magnitude() * length;
		float crossSectionalAreaSide = Vec2(collider->bounds.top - collider->bounds.bottom).magnitude() * length;

		// Drag coefficient.
		const float cd = 0.003f;

		// Calculate volume
		float volume = Vec2(collider->bounds.left - collider->bounds.right).magnitude() * Vec2(collider->bounds.top - collider->bounds.bottom).magnitude() * length;
		
		// Calculate mass density
		float massDensity = mass / volume;

		// Approx. air density.
		const float airDensity = 1.2;

		// Calculate the drag force
		float dragY = .5f * airDensity * crossSectionalAreaBottom * cd * powf(velocity.y, 2.f);
		float dragX = .5f * airDensity * crossSectionalAreaSide * cd * powf(velocity.x, 2.f);

		return Vec2(dragX, dragY);
	}

protected:
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

		// Integrate velocity into position over time.
		position += velocity * time;

		// Update the collider coords.
		updateRectBounds();

		if (other != nullptr && collider->isColliding(other))
		{
			position -= velocity * time;

			// Prevent velocity from constantly zeroing - we only want to zero it on contact.
			// TODO: Account for restitution.
			if (!overlap) 
			{
				velocity = Vec2();
			}

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
		if (isKinematic)
		{
			return;
		}

		velocity += (force - airDrag()) / mass;
	}
};