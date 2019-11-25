#include "Plane.h"
#include <Gizmos.h>

/************************************************************************************************************
************************************************************************************************************/
Plane::Plane(glm::vec2 normal, float height, float elasticity) : PhysicsObject(PLANE, elasticity) {
	m_normal = glm::normalize(normal);
	m_distFromOrigin = height;
}

/************************************************************************************************************
************************************************************************************************************/
Plane::~Plane() {

}

/************************************************************************************************************
************************************************************************************************************/
void Plane::fixedUpdate(glm::vec2 gravity, float timeStep) {

}

/************************************************************************************************************
************************************************************************************************************/
void Plane::resolveCollision(RigidBody* other, glm::vec2 contact) {
	glm::vec2 velocity = other->getVelocity();
	float elasticity = other->getElasticity();
	float j = glm::dot(-(1 + elasticity)*velocity, m_normal) / (1 / other->getMass());

	glm::vec2 force = m_normal * j;

	other->applyForce(force, contact - other->getPosition());
}

/************************************************************************************************************
************************************************************************************************************/
void Plane::makeGizmo() {
	float lineSegmentLength = 3000;
	glm::vec2 centerPoint = m_normal * m_distFromOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel*lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel*lineSegmentLength);
	
	aie::Gizmos::add2DLine(start, end, colour);
}