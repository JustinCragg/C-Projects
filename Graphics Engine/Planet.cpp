#include "Planet.h"
#include "Application3D.h"
#include <Gizmos.h>
#include <glm\ext.hpp>

/************************************************************************************************************
************************************************************************************************************/
Planet::Planet(glm::vec3 localPos, float radius, float orbitSpeed, glm::vec4 colour, SceneObject* parent) : SceneObject(localPos, glm::vec3(0), glm::vec3(1), parent) {
	m_radius = radius;
	m_colour = colour;

	m_orbitAngle = 0;
	m_orbitSpeed = orbitSpeed;
	m_orbitDistance = glm::length(m_localPos);

	m_boundingSphere = new BoundingSphere(glm::vec3(0), m_radius);
}

/************************************************************************************************************
************************************************************************************************************/
Planet::~Planet() {}

/************************************************************************************************************
************************************************************************************************************/
void Planet::update() {
	float pi = glm::pi<float>();
	m_orbitAngle += Application3D::getDeltaTime() * m_orbitSpeed;

	m_localPos.x = glm::sin(pi + m_orbitAngle) * m_orbitDistance;
	m_localPos.z = glm::cos(pi + m_orbitAngle) * m_orbitDistance;

	SceneObject::update();
}

/************************************************************************************************************
************************************************************************************************************/
void Planet::updateGizmos(SceneObject* _camera) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		if (onScreen(_camera, m_boundingSphere) == true) {
			addGizmo();
		}
		SceneObject::updateGizmos(_camera);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void Planet::addGizmo() {
	aie::Gizmos::addSphere(m_worldTransform[3], m_radius, (int)m_radius + 10, (int)m_radius + 10, m_colour, &m_worldTransform);
}