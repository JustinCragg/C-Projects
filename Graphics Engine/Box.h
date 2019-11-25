#pragma once
#include "SceneObject.h"

// A class for drawing box Gizmos
class Box : public SceneObject {
public:
    // Standard constructor - extents is the full width, height, depth
	Box(glm::vec3 localPos, glm::vec3 extents, glm::vec4 colour, SceneObject* parent);
    // Standard destructor
	~Box();

    // Handles the updating of this object's children
    // Rotates the box through a quaternion SLERP
	virtual void update();
    // Handles the updating of Gizmos for this object's children
	virtual void updateGizmos(SceneObject* _camera);

protected:
    // Handles the creation of this object's Gizmo
	virtual void addGizmo();

    // The stage of SLERP the box is currently at
	float m_step = 0;
    // Whether the m_step is increasing or decreasing
	float m_direction = 1;

    // The full extens of the box
	glm::vec3 m_extents;
    // The colour of the box
	glm::vec4 m_colour;
};