#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

// A Spring class which applies forces to the connected objects according to Hooke's Law
class SpringJoint : public PhysicsObject {
public:
	// The standard constructor
	SpringJoint(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float damping = 0.1f, glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));

	// Handles the applying of the forces
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

	// The relative position to m_body1 one side of the joint is attached to
	// Not working
	glm::vec2 m_contact1;
	// The relative position to m_body2 one side of the joint is attached to
	// Not working
	glm::vec2 m_contact2;

	// Determines when the applied force will be negated due to being to small
	float m_damping;
	// The resting distance of the spring
	float m_restLength;
	// Determines how strongly the spring attempts to reach the rest length
	float m_springCoefficient;

	// Whether the spring will apply forces to the bodies
	bool m_active = true;
};