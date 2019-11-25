#include "SpringJoint.h"
#include <Gizmos.h>

/************************************************************************************************************
*************************************************************************************************************/
SpringJoint::SpringJoint(RigidBody* body1, RigidBody* body2, float restLength, float springCoefficient, float damping, glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(JOINT, 0.9f) {
	m_body1 = body1;
	m_body2 = body2;
	m_restLength = restLength;
	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_contact1 = contact1;
	m_contact2 = contact2;
}

/************************************************************************************************************
*************************************************************************************************************/
void SpringJoint::fixedUpdate(glm::vec2 gravity, float timeStep) {
	glm::vec2 p1 = m_body1->toWorld(m_contact1);
	glm::vec2 p2 = m_body2->toWorld(m_contact2);
	glm::vec2 dist = p2 - p1;
	float length = sqrtf(dist.x*dist.x + dist.y*dist.y);

	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();

	// Hooke's Law
	// F = -kX - bv
	glm::vec2 force = dist * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
}

/************************************************************************************************************
*************************************************************************************************************/
void SpringJoint::makeGizmo() {
	aie::Gizmos::add2DLine(m_body1->toWorld(m_contact1), m_body2->toWorld(m_contact2), glm::vec4(1,1,1,1));
}