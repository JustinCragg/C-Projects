#pragma once
#include "Sprite.h"
#include "Asteroid.h"
#include "SceneObject.h"

// Handles the collision between Sprites
// Returns true if there is a collision
// Collision will not be tested if the objects cannot touch
bool handleCollision(Sprite& a, Sprite& b);

// Tests collision between two boxes
// Returns true if there is a collision
bool boxBoxCollision(Collider& aCol, Collider& bCol);
// Tests collision between a box and a circle
// The first collider must be a box
// The second collider must be a circle
// Returns true if there is a collision
bool boxCircleCollision(Collider& aCol, Collider& bCol);
// Tests collision between two circles
// Returns true if there is a collision
bool circleCircleCollision(Collider& aCol, Collider& bCol);

// Bounces two asteroids off each other
void bounce(Asteroid& a, Asteroid& b, float deltaTime);

// Returns the float distance between two SceneObjects
// Returns the distance squared - does not use squareroot
float distanceBetweenSqr(SceneObject& a, SceneObject& b);

// Clamps the value to between the min and max
float clamp(float value, float min, float max);