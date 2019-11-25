#pragma once

#include <Gizmos.h>
#include <Input.h>
#include <glm\ext.hpp>

class Box {
public:
	void init(glm::vec2 pos, glm::vec2 extents, glm::vec4 colour);
	void update();

	bool m_active = true;

	glm::vec2 m_pos{ 0,0 };
	glm::vec2 m_extents{ 10, 7 };
	glm::vec4 m_colour = glm::vec4(1, 1, 1, 1);
protected:
};