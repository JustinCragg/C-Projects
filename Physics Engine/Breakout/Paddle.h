#pragma once
#include <Gizmos.h>
#include <Input.h>
#include <glm\ext.hpp>
#include "Box.h"

class Paddle : public Box {
public:
	void update(aie::Input* input, float deltaTime);

	float moveSpeed = 400;
protected:
};