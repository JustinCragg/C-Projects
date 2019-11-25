#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>

class Ball {
public:
	void init(glm::vec2 pos, float radius, glm::vec4 colour);
	void update(float deltaTime);

	glm::vec2 m_pos{ 0,0 };
	float m_radius = 15;
	glm::vec4 m_colour = glm::vec4(1, 0.9f, 0, 1);

	bool m_active = true;

	glm::vec2 m_direc{ 1,1 };
	float m_speed = 300;
protected:
};