#pragma once
#include "PhysicsObject.h"
#include "Sphere.h"
#include "SpringJoint.h"
#include <vector>

// The base class for objects which are created using multiple particles - in order to allow for deformation
class SoftBody : public PhysicsObject {
public:
	// The standard constructor
	SoftBody(float elasticty, glm::vec2 pos, float mass, float strength, glm::vec4 colour = glm::vec4(0,0,0,0));
	//The standard destructor
	~SoftBody();

	// Calls the fixedUpdate for each of the particles and springs forming the object
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	// This function can be used to debug an instance of this class
	virtual void debug() {};
	// Calls the makeGizmo for each of the particles and springs
	virtual void makeGizmo();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the color of the object
	glm::vec4 getColour() { return m_colour; }
	// Returns a list of pointers to each of the particles making up the object
	std::vector<Sphere*> getParticles() { return m_particles; };

protected:
	// Handles the layout and creation of the particles and springs which make up the object
	// Must be overriden by child classes
	virtual void init(glm::vec2 velocity = glm::vec2(0, 0)) = 0;

	// The particles which make up the object
	std::vector<Sphere*> m_particles;
	// The springJoints which make up the object
	std::vector<SpringJoint*> m_joints;

	// The position if the object
	glm::vec2 m_pos;

	// The strength of the springs holding the object together
	float m_strength;

	// The mass of the object
	float m_mass;
	
	// The colour of the object
	glm::vec4 m_colour;
};