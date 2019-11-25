#include "RigidBody.h"
#include <iostream>

/************************************************************************************************************
*************************************************************************************************************/
RigidBody::RigidBody(ShapeType shapeID, glm::vec2 pos, float mass, float rotation, glm::vec2 velocity, float elasticity) : PhysicsObject(shapeID, elasticity) {
	m_pos = pos;
	m_velocity = velocity;
	m_angularVelocity = 0;
	m_rot = rotation;
	m_mass = mass;

	m_localX = glm::vec2(1, 0);
	m_localY = glm::vec2(0, 1);
}

/************************************************************************************************************
*************************************************************************************************************/
RigidBody::~RigidBody() {
}

/************************************************************************************************************
*************************************************************************************************************/
void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos) {
	if (m_isKinematic == false) {
		m_velocity += force / m_mass;
		m_angularVelocity += (force.y * pos.x - force.x * pos.y) / m_moment;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void RigidBody::resolveCollision(RigidBody* other, glm::vec2 contact, glm::vec2* collisionNormal) {
	glm::vec2 normal;
	if (collisionNormal != nullptr) {
		normal = glm::normalize(*collisionNormal);
	}
	else {
		normal = glm::normalize(other->m_pos - m_pos);
	}
	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_pos, -perp);
	float r2 = glm::dot(contact - other->m_pos, perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(other->m_velocity, normal) + r2 * other->m_angularVelocity;

	// If the objects are not moving apart
	if (v1 > v2) {
		float mass1 = 1.0f / (1.0f / getMass() + (r1*r1) / getMoment());
		float mass2 = 1.0f / (1.0f / other->getMass() + (r2*r2) / other->getMoment());

		float elasticity = (m_elasticity + other->m_elasticity) / 2.0f;

		glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / (mass1 + mass2)*(v1 - v2)*normal;

		applyForce(-force, contact - m_pos);
		other->applyForce(force, contact - other->m_pos);
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep) {
	if (m_isKinematic == true) {
		return;
	}

	// Apply Gravity
	m_velocity += gravity * timeStep;
	// Modify Position and Rotation
	m_pos += m_velocity * timeStep;
	m_rot += m_angularVelocity * timeStep;

	// Apply drag
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (glm::length(m_velocity) < 0.01f) {
		m_velocity = glm::vec2(0, 0);
	}
	if (glm::length(m_angularVelocity) < 0.001f) {
		m_angularVelocity = 0;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
float RigidBody::getKineticEnergy() {
	return 0.5f * (m_mass*glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity*m_angularVelocity);
}

/************************************************************************************************************
************************************************************************************************************/
glm::vec2 RigidBody::toWorld(glm::vec2 point) {
	glm::vec2 newPoint(m_pos + m_localX * point.x + m_localY * point.y);
	return newPoint;
}

/************************************************************************************************************
*************************************************************************************************************/
void RigidBody::debug() {
	std::cout << glm::length(m_velocity) << std::endl;
}