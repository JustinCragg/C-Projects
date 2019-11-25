#include "Ball.h"

/************************************************************************************************************
*************************************************************************************************************/
void Ball::init(glm::vec2 pos, float radius, glm::vec4 colour) {
	m_pos = pos;
	m_radius = radius;
	m_colour = colour;
}

/************************************************************************************************************
*************************************************************************************************************/
void Ball::update(float deltaTime) {
	if (m_active == true) {
		m_pos += m_direc * m_speed * deltaTime;
		if (m_pos.x < -640) {
			m_direc.x *= -1;
		}
		else if (m_pos.x > 640) {
			m_direc.x *= -1;
		}
		if (m_pos.y < -360) {
			m_active = false;
		}
		else if (m_pos.y > 360) {
			m_direc.y *= -1;
		}

		aie::Gizmos::add2DCircle(m_pos, m_radius, (int)m_radius, m_colour);
	}
}