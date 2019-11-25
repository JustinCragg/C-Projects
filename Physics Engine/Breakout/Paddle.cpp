#include "Paddle.h"

/************************************************************************************************************
*************************************************************************************************************/
void Paddle::update(aie::Input* input, float deltaTime) {
	if (input->isKeyDown(aie::INPUT_KEY_LEFT)) {
		m_pos.x -= moveSpeed * deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT)) {
		m_pos.x += moveSpeed * deltaTime;
	}

	Box::update();
}