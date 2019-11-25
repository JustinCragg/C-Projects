#include "FlyCamera.h"
#include "Application3D.h"
#include <Input.h>
#include <glm/ext.hpp>

#include <iostream>

/************************************************************************************************************
************************************************************************************************************/
FlyCamera::FlyCamera(glm::vec3 localPos, float aspectRatio, SceneObject* parent) : Camera (localPos, parent) {
	m_up = glm::vec3(0, 1, 0);
	m_speed = 50.0f;
	setLookAt(m_worldTransform[2], glm::vec3(0), m_up);
	setPerspective(glm::pi<float>() * 0.25f, aspectRatio, 0.1f, 1000.f);
}

/************************************************************************************************************
************************************************************************************************************/
FlyCamera::~FlyCamera() {

}

/************************************************************************************************************
************************************************************************************************************/
void FlyCamera::update() {
    aie::Input* input = aie::Input::getInstance();
	float deltaTime = Application3D::getDeltaTime();

	// Shift 'sprint'
	float normalSpeed = m_speed;
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
		m_speed *= 3;
	}
	// Movement
	if (input->isKeyDown(aie::INPUT_KEY_W)) {
		glm::vec3 delta = m_worldTransform[2] * deltaTime * m_speed;
		m_localPos += delta;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S)) {
		glm::vec3 delta = -m_worldTransform[2] * deltaTime * m_speed;
		m_localPos += delta;
	}
	if (input->isKeyDown(aie::INPUT_KEY_A)) {
		glm::vec3 delta = m_worldTransform[0] * deltaTime * m_speed;
		m_localPos += delta;
	}
	if (input->isKeyDown(aie::INPUT_KEY_D)) {
		glm::vec3 delta = -m_worldTransform[0] * deltaTime * m_speed;
		m_localPos += delta;
	}
	// Rotation
	if (input->isKeyDown(aie::INPUT_KEY_LEFT)) {
		m_localRot.y += deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT)) {
		m_localRot.y += -deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_UP)) {
		m_localRot.x += -deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_DOWN)) {
		m_localRot.x += deltaTime;
	}
	m_speed = normalSpeed;

	SceneObject::update();

	glm::vec4 lookPos = m_worldTransform[3] + m_worldTransform[2];
	setLookAt(m_worldTransform[3], lookPos, m_up);
}