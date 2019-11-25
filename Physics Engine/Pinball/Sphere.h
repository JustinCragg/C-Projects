#pragma once
#include "RigidBody.h"

// A Rigidbody class for 2d spheres
class Sphere : public RigidBody {
public:
	// Defaults: mass=0, radius=15, velocity=(0,0), elasticity=0.99f, colour=random
	Sphere(glm::vec2 pos);
	// Defaults: velocity=(0,0), colour=random
	Sphere(glm::vec2 pos, float mass, float radius, float elasticity = 0.99f);
	// Defaults: colour=random
	Sphere(glm::vec2 pos, float mass, float radius, glm::vec2 velocity, float elasticity=0.99f);
	// Initialises the box class
	Sphere(glm::vec2 pos, float mass, float radius, glm::vec2 velocity, float elasticity, glm::vec4 colour);
	// The standard destructor
	~Sphere();

	// Assigns the sphere to be drawn
	virtual void makeGizmo();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the radius of the sphere
	float getRadius() { return m_radius; }
	// Returns the colour of the sphere
	glm::vec4 getColour() { return m_colour; }

protected:
	// The radius of the sphere
	float m_radius;
	// The colour of the sphere
	glm::vec4 m_colour;
};