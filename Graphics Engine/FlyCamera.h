#pragma once
#include "Camera.h"

// Class for a camera which moves around the scene according to keyboard input
class FlyCamera : public Camera {
public:
    // Standard constructor
	FlyCamera(glm::vec3 localPos, float aspectRatio, SceneObject* parent);
    // Standard constructor
	~FlyCamera();

    // Handles the movement and input for the camera
    // WASD - movement, Arrow Keys - rotation
	virtual void update();

    // Sets the speed the camera moves at
	void setSpeed(float speed) { m_speed = speed; }

protected:
	// The world's up (0,1,0)
	glm::vec3 m_up;

    // The speed the camera moves and rotates at
	float m_speed;
};