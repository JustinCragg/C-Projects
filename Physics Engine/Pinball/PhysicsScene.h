#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <vector>
#include "Bumper.h"

class PhysicsObject;

// A class which handles the physics for all objects
class PhysicsScene {
public:
	// The standard constructor
	PhysicsScene();
	// The standard destructor
	~PhysicsScene();

	// Add a physics object to the scene
	void addActor(PhysicsObject* actor);
	// Removes a physics object from the scene
	void removeActor(PhysicsObject* actor);
	// Adds the individual components of a bumper to the scene
	void addBumper(Bumper* bumper);
	// Calls the fixedUpdate and checks collision for each actor in the scene
	void update(float deltaTime);
	// Calls the updateGizmo for each actor
	void updateGizmos();
	// Calls the debug for each actor
	void debugScene();

	// Checks which objects are flagged for removal, and removes them
	void checkRemoves();

	// Handles all collisions between objects
	void checkForCollision();
	// Handles plane to plane collision - empty
	static bool plane2Plane(PhysicsObject* objA, PhysicsObject* objB);
	// Handles plane to sphere collision
	static bool plane2Sphere(PhysicsObject* objA, PhysicsObject* objB);
	// Handles plane to box collision
	static bool plane2Box(PhysicsObject* objA, PhysicsObject* objB);
	// Handles sphere to plane collision
	static bool sphere2Plane(PhysicsObject* objA, PhysicsObject* objB);
	// Handles sphere to sphere collision
	static bool sphere2Sphere(PhysicsObject* objA, PhysicsObject* objB);
	// Handles sphere to box collision
	static bool sphere2Box(PhysicsObject* objA, PhysicsObject* objB);
	// Handles box to plane collision
	static bool box2Plane(PhysicsObject* objA, PhysicsObject* objB);
	// Handles box to sphere collision
	static bool box2Sphere(PhysicsObject* objA, PhysicsObject* objB);
	// Handles box to box collision
	static bool box2Box(PhysicsObject* objA, PhysicsObject* objB);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Sets the gravity which affects the objects in the scene
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	// Sets the time step for the simulation
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the gravity of the scene
	glm::vec2 getGravity() const { return m_gravity; }
	// Returns the time step for scene
	float getTimeStep() const { return m_timeStep; }

protected:
	// The gravity affecting the objects in the scene
	glm::vec2 m_gravity;
	// The time step of the fixedUpdate
	float m_timeStep;
	// The actors in the scene
	std::vector<PhysicsObject*> m_actors;

	// Counts the amount of time between when sounds should be played
	float m_soundCounter = 0;
	// Has a sound been played recently
	bool m_soundPlayed = false;
};