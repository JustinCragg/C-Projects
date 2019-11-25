#pragma once
#include "SpringJoint.h"
#include "Box.h"

// A class used in the pinball simulation
// Contains a kinematic box attached to another box by a spring
class Bumper {
public:
	// The standard constructor
	Bumper(glm::vec2 fixedPoint, float rot, glm::vec2 bumperSize, float strength);
	// The standard destructor
	~Bumper();

	// Resets the position of the dynamic box
	void resetBumper();

	// The kinematic box which is the centre of the bumper
	Box* m_fixedPoint;
	// The dynamic box attached to the centre
	Box* m_bumper;
	// The spring connecting the two boxes
	SpringJoint* m_spring;

protected:
	// The direction the bumper is facing
	glm::vec2 m_direc;

	// The rotation of the bumper
	float m_rot;
};