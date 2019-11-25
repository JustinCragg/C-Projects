#pragma once
#include "SoftBody.h"

// A Softbody class for 2d spheres
class SoftSphere : public SoftBody {
public:
	// Defaults: mass=1, radius=15, strength=10
	SoftSphere(glm::vec2 pos);
	// Initialises the soft sphere class
	SoftSphere(glm::vec2 pos, float mass, float radius, float strength);
	// The standard destructor
	~SoftSphere();

	// Calls the SoftBody makeGizmo
	virtual void makeGizmo();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the radius of the object
	float getRadius() { return m_radius; }

protected:
	// Handles the layout and creation of the particles and springs which make up the object
	virtual	void init(glm::vec2 velocity = glm::vec2(0,0));

	// The radius of the object
	float m_radius;
};