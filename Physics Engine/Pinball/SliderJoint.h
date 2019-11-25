#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

// A Joint class which restricts the movement of the attached objects along a single axis
class SliderJoint : public PhysicsObject {
public:
	// The standard constructor
	SliderJoint(RigidBody* body1, RigidBody* body2, glm::vec2 direc, float minLength, float maxLength);

	// Handles the restriction of the positions
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	// This function can be used to debug an instance of this class
	virtual void debug() {}
	// Handles the drawing of the joint
	virtual void makeGizmo();

protected:
	// The first object the joint is attached to
	RigidBody* m_body1;
	// The second object the joint is attached to
	RigidBody* m_body2;

	// The axis the objects will be constrained to
	glm::vec2 m_direc;

	// The minimum length the objects must be apart by
	float m_minLength;
	// The maximum length the objects must be apart by
	float m_maxLength;
};