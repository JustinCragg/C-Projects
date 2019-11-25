#include "Box.h"
#include "Application3D.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <iostream>

/************************************************************************************************************
************************************************************************************************************/
Box::Box(glm::vec3 localPos, glm::vec3 extents, glm::vec4 colour, SceneObject* parent) : SceneObject(localPos, glm::vec3(0), glm::vec3(1), parent) {
	m_extents = extents;
	m_colour = colour;

    float radius = glm::sqrt(extents.x*extents.x + extents.y*extents.y + extents.z*extents.z);

	m_boundingSphere = new BoundingSphere(glm::vec3(0), radius);
}

/************************************************************************************************************
************************************************************************************************************/
Box::~Box() {}

/************************************************************************************************************
************************************************************************************************************/
void Box::update() {
	m_step = glm::clamp(m_step + Application3D::getDeltaTime()*0.1f * m_direction, 0.0f, 1.0f);
	if (m_step == 1.0f || m_step == 0) {
		m_direction *= -1;
	}
	glm::quat startRot(glm::vec3(glm::pi<float>() * 0, glm::pi<float>() * 0, glm::pi<float>() * 0));
	glm::quat endRot(glm::vec3(glm::pi<float>() * 1, glm::pi<float>() * 0.5f, glm::pi<float>() * 0.25f));

	glm::quat currentRot = glm::slerp(startRot, endRot, m_step);

	m_localRot = glm::eulerAngles(currentRot);

	SceneObject::update();
}

/************************************************************************************************************
************************************************************************************************************/
void Box::updateGizmos(SceneObject* _camera) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		if (onScreen(_camera, m_boundingSphere) == true) {
			addGizmo();
		}
		SceneObject::updateGizmos(_camera);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void Box::addGizmo() {
	aie::Gizmos::addAABB(m_worldTransform[3], m_extents, m_colour, &m_worldTransform);
}