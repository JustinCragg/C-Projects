#pragma once
#include <string>

class Need {
public:
	Need(std::string name) { m_name = name; }

	std::string m_name;
	float m_value = 1.0f;
	float m_decay;
};