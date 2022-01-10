#pragma once

#include "constraint.h"

enum ConstraintType { Spring };

class SpringConstraint : public Constraint
{
public:
	SpringConstraint(std::initializer_list<RBDynamic*> attachedBodies, float maxLength = 100.f, float equilibrium = 50.f, float damping = 0.15f, float stiffness = 100.f) : Constraint(ConstraintType::Spring, attachedBodies)
	{
		this->maxLength = maxLength;
		this->equilibrium = equilibrium;
		this->damping = damping;
		this->stiffness = stiffness;
	}

	void step(float time)
	{
		Vec2 toParent = attachedBodies[0]->position - attachedBodies[1]->position;

		float distBetweenBodies = toParent.magnitude();

		Vec2 springForce = toParent.normalised() * (distBetweenBodies - equilibrium) * stiffness;

		if (distBetweenBodies > maxLength)
		{
			//attachedBodies[1]->position = attachedBodies[0]->position.y > attachedBodies[1]->position.y ? attachedBodies[0]->position : attachedBodies[0]->position - toParent.normalised() * maxLength;
			attachedBodies[1]->position.y = attachedBodies[0]->position.y + maxLength;
			attachedBodies[1]->velocity = Vec2();
		}
		else
		{
			attachedBodies[1]->addForce(springForce);
		}

		attachedBodies[1]->updateRectBounds();

		std::cout << "maxLength: " << maxLength << ", current stretch: " << distBetweenBodies << std::endl;
		std::cout << "spring force: " << springForce.magnitude() << std::endl;
	}

	float maxLength, equilibrium, damping, stiffness;
};