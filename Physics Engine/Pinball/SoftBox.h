#pragma once
#include "SoftBody.h"

// A Softbody class for 2d boxes
class SoftBox : public SoftBody {
public:
	// Defaults: mass=1, halfExtents=(15,15), strength=10
	SoftBox(glm::vec2 pos);
	// Initialises the soft box class
	SoftBox(glm::vec2 pos, float mass, glm::vec2 halfExtents, float strength);
	// The standard destructor
	~SoftBox();

	// Calls the SoftBody makeGizmo
	virtual void makeGizmo();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the half extents of the object
	glm::vec2 getHalfExtents() { return m_halfExtents; }

protected:
	// Handles the layout and creation of the particles and springs which make up the object
	virtual void init(glm::vec2 velocity = glm::vec2(0, 0));

	// The half extents of the object - the distance from the centre to the edge
	glm::vec2 m_halfExtents;
};