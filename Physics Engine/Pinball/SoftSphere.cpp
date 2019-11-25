#include "SoftSphere.h"

#include <Gizmos.h>
#define _USE_MATH_DEFINES
#include <math.h>

/************************************************************************************************************
************************************************************************************************************/
SoftSphere::SoftSphere(glm::vec2 pos) : SoftBody(0.99f, pos, 1, 10) {
	m_radius = 15;

	init();
}

/************************************************************************************************************
************************************************************************************************************/
SoftSphere::SoftSphere(glm::vec2 pos, float mass, float radius, float strength) : SoftBody(0.99f, pos, mass, strength) {
	m_radius = radius;

	init();
}

/************************************************************************************************************
************************************************************************************************************/
void SoftSphere::init(glm::vec2 velocity) {
	float ringAmount = 0;
	int particleCount = 1;
	for (int i = 0; i < m_radius / 5.0f; i++) {
		ringAmount = 2 * (float)M_PI * i;
		for (int j = 0; j < ringAmount; j++) {
			particleCount++;
		}
	}
	float size = 5;
	float dampening = 1.0f;
	std::vector<std::vector<Sphere*>> tempVector;
	tempVector.push_back(std::vector<Sphere*>());
	Sphere* particle = new Sphere(m_pos, m_mass / (float)particleCount, size/2.0f, velocity, m_elasticity, glm::vec4(1,0,0,1));
	tempVector[0].push_back(particle);

	// Creates particles from the centre, looping around at different radi
	for (int i = 1; i < m_radius / size + 1; i++) {
		ringAmount = 2 * (float)M_PI * i;
		tempVector.push_back(std::vector<Sphere*>());
		for (int j = 0; j < ringAmount/2; j++) {
			float angle = (2 * (float)M_PI / (ringAmount / 2 + 1)) * j;
			glm::vec2 pos(cos(angle), sin(angle));
			Sphere* particle = new Sphere(m_pos + pos * (float)i*size, m_mass / (float)particleCount, size / 2.0f, velocity, m_elasticity, m_colour);
			tempVector[i].push_back(particle);
		}
		for (int k = 0; k < tempVector[i].size(); k++) {
			// Connect to previous
			if (k == 0) {
				m_joints.push_back(new SpringJoint(tempVector[i][k], tempVector[i][tempVector[i].size()-1], glm::length(tempVector[i][k]->getPosition() - tempVector[i][tempVector[i].size() - 1]->getPosition())+5, m_strength, dampening));
			}
			else {
				m_joints.push_back(new SpringJoint(tempVector[i][k], tempVector[i][k - 1], glm::length(tempVector[i][k]->getPosition() - tempVector[i][k - 1]->getPosition()), m_strength, dampening));
			}
			// Connect to k/2.0f rounded up
			if (i == 1) {
				m_joints.push_back(new SpringJoint(tempVector[i][k], tempVector[i-1][0], glm::length(tempVector[i][k]->getPosition() - tempVector[i-1][0]->getPosition()), m_strength, dampening));
			}
			else {
				m_joints.push_back(new SpringJoint(tempVector[i][k], tempVector[i - 1][(int)round(k / 2.0f)], glm::length(tempVector[i][k]->getPosition() - tempVector[i - 1][(int)round(k / 2.0f)]->getPosition()), m_strength, dampening));
			}
		}
	}

	for each (std::vector<Sphere*> vector in tempVector) {
		for each (Sphere* sphere in vector) {
			m_particles.push_back(sphere);
		}
	}
}

/************************************************************************************************************
************************************************************************************************************/
SoftSphere::~SoftSphere() {

}

/************************************************************************************************************
************************************************************************************************************/
void SoftSphere::makeGizmo() {
	SoftBody::makeGizmo();
	//aie::Gizmos::add2DCircle(m_pos, m_radius, (int)m_radius + 3, m_colour);
}