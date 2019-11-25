#pragma once
#include "RigidBody.h"
#include <vector>

// A Rigidbody class for 2d rotatable boxs
class Box : public RigidBody {
public:
	// Defaults: rotation=0, mass=1, halfExents=(15,15), velocity=(0,0), elasticity=0.99f, colour=random
	Box(glm::vec2 pos);
	// Defaults: mass=1, halfExents=(15,15), velocity=(0,0), elasticity=0.99f, colour=random
	Box(glm::vec2 pos, float rotation);
	// Defaults: velocity=(0,0), colour=random
	Box(glm::vec2 pos, float rotation, float mass, glm::vec2 halfExtents, float elasticity=0.99f);
	// Initialises the box class
	Box(glm::vec2 pos, float rotation, float mass, glm::vec2 halfExtents, glm::vec2 velocity, float elasticity, glm::vec4 colour);
	// The standard destructor
	~Box();

	// Calls the rigidbody fixed update
	// Redetermines local axes
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	// Assigns the box to be drawn
	virtual void makeGizmo();

	// Checks whether this box's corners collide with the given box
	// Modifies the variables passed in
	bool checkBoxCorners(Box* box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal, glm::vec2& contactForce);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the half extents of the box
	glm::vec2 getHalfExtents() { return m_halfExtents; }
	// Returns the colour of the box
	glm::vec4 getColour() { return m_colour; }

protected:
	// The half extents of the box - the distance from the centre to the edge
	glm::vec2 m_halfExtents;
	// The colour of the box
	glm::vec4 m_colour;
};