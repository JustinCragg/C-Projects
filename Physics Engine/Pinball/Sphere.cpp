#include "Sphere.h"
#include <Gizmos.h>

/************************************************************************************************************
*************************************************************************************************************/
Sphere::Sphere(glm::vec2 pos) : RigidBody(SPHERE, pos, 1, 0, glm::vec2(0,0), 0.99f) {
	m_radius = 15;
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

/************************************************************************************************************
*************************************************************************************************************/
Sphere::Sphere(glm::vec2 pos, float mass, float radius, float elasticity) : RigidBody(SPHERE, pos, mass, 0, glm::vec2(0,0), elasticity) {
	m_radius = radius;
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

/************************************************************************************************************
*************************************************************************************************************/
Sphere::Sphere(glm::vec2 pos, float mass, float radius, glm::vec2 velocity, float elasticity) : RigidBody(SPHERE, pos, mass, 0, velocity, elasticity) {
	m_radius = radius;
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

/************************************************************************************************************
*************************************************************************************************************/
Sphere::Sphere(glm::vec2 pos, float mass, float radius, glm::vec2 velocity, float elasticity, glm::vec4 colour) : RigidBody(SPHERE, pos, mass, 0, velocity, elasticity) {
	m_radius = radius;
	m_colour = colour;

	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

/************************************************************************************************************
*************************************************************************************************************/
Sphere::~Sphere() {
}

/************************************************************************************************************
*************************************************************************************************************/
void Sphere::makeGizmo() {
	glm::vec2 end = glm::vec2(std::cos(m_rot), std::sin(m_rot))*m_radius;

	aie::Gizmos::add2DCircle(m_pos, m_radius, (int)m_radius+3, m_colour);
	aie::Gizmos::add2DLine(m_pos, m_pos + end, m_colour*0.5f);
}