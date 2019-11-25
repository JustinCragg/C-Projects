#pragma once
#include "SceneObject.h"

// Class for drawing sphere Gizmos which act as orbiting bodies
class Planet : public SceneObject {
public:
    // Standard constructor - orbits around parent
	Planet(glm::vec3 localPos, float radius, float orbitSpeed, glm::vec4 colour, SceneObject* parent);
    // Default destructor
	~Planet();

    // Handles the updating of this object's children
    // Orbits the planet around the parent
	virtual void update();
    // Handles the updating of Gizmos for this object's children
	virtual void updateGizmos(SceneObject* _camera);

protected:
    // Handles the creation of this object's Gizmo
	virtual void addGizmo();

    // The radius of the sphere
	float m_radius;
    // The colour of the sphere
	glm::vec4 m_colour;

    // The angle planet is at relative to its orbit
	float m_orbitAngle;
    // The speed the planet orbits at
	float m_orbitSpeed;
    // The relative distance from the parent object the planet orbits at
	float m_orbitDistance;
};