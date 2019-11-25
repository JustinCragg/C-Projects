#pragma once
#include "Physics.h"
#include <glm\ext.hpp>

bool boxBallCollision(Box* box, Ball* ball) {
	glm::vec2 point = box->m_pos - ball->m_pos;
	point = glm::normalize(point);
	point *= ball->m_radius;
	point += ball->m_pos;

	float left = box->m_pos.x - box->m_extents.x;
	float right = box->m_pos.x + box->m_extents.x;
	float top = box->m_pos.y + box->m_extents.y;
	float bottom = box->m_pos.y - box->m_extents.y;

	if (point.x < right && point.x > left) {
		if (point.y < top && point.y > bottom) {
			ball->m_direc.x *= 1;
			ball->m_direc.y *= -1;

			return true;
		}
	}

	return false;
}