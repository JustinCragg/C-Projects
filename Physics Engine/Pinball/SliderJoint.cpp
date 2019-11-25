#include "SliderJoint.h"
#include <Gizmos.h>

/************************************************************************************************************
*************************************************************************************************************/
SliderJoint::SliderJoint(RigidBody* body1, RigidBody* body2, glm::vec2 direc, float minLength, float maxLength) : PhysicsObject(JOINT, 0.9f) {
	m_body1 = body1;
	m_body2 = body2;

	m_direc = direc;

	m_minLength = minLength;
	m_maxLength = glm::max(m_minLength, maxLength);
}

/************************************************************************************************************
*************************************************************************************************************/
void SliderJoint::fixedUpdate(glm::vec2 gravity, float timeStep) {
	glm::vec2 p1 = m_body1->getPosition();
	glm::vec2 p2 = m_body2->getPosition();
	glm::vec2 dist = p2 - p1;
	float length = sqrtf(dist.x*dist.x + dist.y*dist.y);
}

/************************************************************************************************************
*************************************************************************************************************/
void SliderJoint::makeGizmo() {
	aie::Gizmos::add2DLine(m_body1->getPosition(), m_body2->getPosition(), glm::vec4(1, 1, 1, 1));
}