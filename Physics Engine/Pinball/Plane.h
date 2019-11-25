#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

// A straight plane with an infinite length
class Plane : public PhysicsObject {
public:
	// The standard constructor
	Plane(glm::vec2 normal, float dist, float elasticity=0.99f);
	// The standard desstructor
	~Plane();

	// Empty function needed for PhysicsObject
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	// Calculates the forces applied when an object collides with this
	virtual void resolveCollision(RigidBody* other, glm::vec2 contact);
	// Can be used to debug an instance of this object
	virtual void debug() {};
	// Assigns this object to be drawn
	virtual void makeGizmo();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the normal of the plane
	glm::vec2 getNormal() { return m_normal; }
	// Returns the distance away from the centre that this plane is
	float getDistFromOrigin() { return m_distFromOrigin; }

protected:
	// The normal of the plane
	glm::vec2 m_normal;
	// The distance away from the origin this plane is
	float m_distFromOrigin;
};