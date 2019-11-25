#include "SoftBody.h"

/************************************************************************************************************
************************************************************************************************************/
SoftBody::SoftBody(float elasticty, glm::vec2 pos, float mass, float strength, glm::vec4 colour) : PhysicsObject(SOFTBODY, elasticty) {
	m_pos = pos;
	m_mass = mass;
	m_strength = strength;

	if (glm::length(colour) == 0) {
		m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);
	}
	else {
		m_colour = colour;
	}
}

/************************************************************************************************************
************************************************************************************************************/
SoftBody::~SoftBody() {
	for each(Sphere* sphere in m_particles) {
		delete sphere;
	}
	for each(SpringJoint* spring in m_joints) {
		delete spring;
	}
}

/************************************************************************************************************
************************************************************************************************************/
void SoftBody::fixedUpdate(glm::vec2 gravity, float timeStep) {
	int count = 0;
	m_pos = glm::vec2(0, 0);
	// Calls each particles' fixedUpdate
	// Redetermines the object's overall position
	for each (Sphere* sphere in m_particles) {
		sphere->fixedUpdate(gravity, timeStep);
		count++;
		m_pos += sphere->getPosition();
	}
	m_pos /= (float)count;
	// Calls each joints' fixedUpdate
	for each (SpringJoint* spring in m_joints) {
		spring->fixedUpdate(gravity, timeStep);
	}

}

/************************************************************************************************************
************************************************************************************************************/
void SoftBody::makeGizmo() {
	for each (SpringJoint* spring in m_joints) {
		//spring->makeGizmo();
	}
	for each (Sphere* sphere in m_particles) {
		sphere->makeGizmo();
	}
}