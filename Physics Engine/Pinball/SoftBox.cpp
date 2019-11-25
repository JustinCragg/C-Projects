#include "SoftBox.h"

#include <Gizmos.h>
#include <cmath>

/************************************************************************************************************
************************************************************************************************************/
SoftBox::SoftBox(glm::vec2 pos) : SoftBody(0.99f, pos, 1, 10) {
	m_halfExtents = glm::vec2(15, 15);

	init();
}

/************************************************************************************************************
************************************************************************************************************/
SoftBox::SoftBox(glm::vec2 pos, float mass, glm::vec2 halfExtents, float strength) : SoftBody(0.99f, pos, mass, strength) {
	m_halfExtents = halfExtents;

	init();
}

/************************************************************************************************************
************************************************************************************************************/
void SoftBox::init(glm::vec2 velocity) {

	float size = 10;
	float dampening = 1.0f;

	int particleCount = (m_halfExtents.x*2.0f) / size * (m_halfExtents.y*2.0f) / size;

	// Creates particles from top left to bottom right
	for (int x = -m_halfExtents.x; x < m_halfExtents.x; x+=size) {
		for (int y = m_halfExtents.y; y > -m_halfExtents.y; y-=size) {
			glm::vec2 pos = m_pos + glm::vec2(x, y) * 1.0f;
			Sphere* particle = new Sphere(pos, m_mass / particleCount, size*0.25f, velocity, m_elasticity, m_colour);
			m_particles.push_back(particle);
		}
	}
	float numX = m_halfExtents.x * 2 / size;
	float numY = m_halfExtents.y * 2 / size;
	// Creates joints connecting a particles to the particle above and the particle to the left
	for (int y = 0; y < m_particles.size(); y += numX) {
		for (int x = 0; x < numY; x++) {
			if (x != 0) {
				m_joints.push_back(new SpringJoint(m_particles[y + x], m_particles[y + x - 1], glm::length(m_particles[y + x]->getPosition() - m_particles[y + x - 1]->getPosition()), m_strength, dampening));
			}
			if (y != 0) {
				m_joints.push_back(new SpringJoint(m_particles[y + x], m_particles[y - numX + x], glm::length(m_particles[y + x]->getPosition() - m_particles[y - numX + x]->getPosition()), m_strength, dampening));
			}
		}
	}
}

/************************************************************************************************************
************************************************************************************************************/
SoftBox::~SoftBox() {

}

/************************************************************************************************************
************************************************************************************************************/
void SoftBox::makeGizmo() {
	SoftBody::makeGizmo();
}