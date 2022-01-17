#pragma once

#include "constraint.h"

enum ConstraintType { Spring };

class SpringConstraint : public Constraint
{
public:
	SpringConstraint(std::initializer_list<RBDynamic*> attachedBodies, float maxLength = 100.f, float equilibrium = 50.f, float damping = 0.15f, float stiffness = 500.f) : Constraint(ConstraintType::Spring, attachedBodies)
	{
		this->maxLength = maxLength;
		this->equilibrium = equilibrium;
		this->damping = damping;
		this->stiffness = stiffness;
	}

	Vec2 getDampingVelocity(float time)
	{
		// xn: position relative to spring equilibrium
		float relativePos = (attachedBodies[1]->position.y - attachedBodies[0]->position.y) - equilibrium;

		// x'n
		float massVelocity = attachedBodies[1]->velocity.y;

		// Determine a damping coefficient that will return to equilibrium as soon as possible.
		// This is found by the stiffness to mass ratio.
		float wSquared = stiffness / attachedBodies[1]->mass;

		float y = massVelocity - wSquared * time * relativePos;
		y /= std::powf(1 + sqrtf(wSquared) * time, 2.f);

		return Vec2(0.f, y);
	}

	void step(float time)
	{
		Vec2 toEquilibrium = (attachedBodies[0]->position + Vec2(0.f, equilibrium)) - attachedBodies[1]->position;

		float distBetweenBodies = (attachedBodies[0]->position - attachedBodies[1]->position).magnitude();

		Vec2 springForce = Vec2(0.f, toEquilibrium.y * stiffness);

		attachedBodies[1]->addForce(springForce);

		Vec2 dampingForce = getDampingVelocity(time);

		attachedBodies[1]->velocity = dampingForce;

		if (distBetweenBodies > maxLength)
		{
			//attachedBodies[1]->position = attachedBodies[0]->position.y > attachedBodies[1]->position.y ? attachedBodies[0]->position : attachedBodies[0]->position - toParent.normalised() * maxLength;
			attachedBodies[1]->position.y = attachedBodies[0]->position.y + maxLength;
			//attachedBodies[1]->velocity = Vec2();
		}

		attachedBodies[1]->updateRectBounds();

		std::cout << "maxLength: " << maxLength << ", current stretch: " << distBetweenBodies << ", velocity: " << attachedBodies[1]->velocity.y << std::endl;
		std::cout << "spring force: " << springForce.y << std::endl;
	}

	float maxLength, equilibrium, damping, stiffness;
};