#pragma once

#include <vector>
#include "rigidbody.h"

enum ConstraintType : int;

class Constraint 
{
private:
	ConstraintType type;
public:
	virtual void step(float time) = 0;
	ConstraintType getType() { return type; }
	
	std::vector<RBDynamic*> attachedBodies;

protected:
	Constraint(ConstraintType constraintType, std::initializer_list<RBDynamic*> attachedBodies)
	{
		type = constraintType;
		this->attachedBodies.assign(attachedBodies);
	}
};