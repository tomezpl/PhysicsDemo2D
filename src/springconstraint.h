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

	void step(float time)
	{
		Vec2 toEquilibrium = (attachedBodies[0]->position + Vec2(0.f, equilibrium)) - attachedBodies[1]->position;

		float distBetweenBodies = (attachedBodies[0]->position - attachedBodies[1]->position).magnitude();

		Vec2 springForce = Vec2(0.f, toEquilibrium.y * stiffness);

		attachedBodies[1]->addForce(springForce);

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