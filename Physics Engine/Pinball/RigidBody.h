#pragma once
#include "PhysicsObject.h"

// The base class for objects with zero deformation
class RigidBody : public PhysicsObject {
public:
	// The standard constructor
	RigidBody(ShapeType shapeID, glm::vec2 pos, float mass, float rotation, glm::vec2 velocity, float elasticity);
	// The standard destructor
	virtual ~RigidBody();

	// Handles the applying of velocity to this object
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	// This function can be used to debug an instance of this class
	virtual void debug();

	// Applies a force to this object at the position
	// Affects positional and rotational velocity
	void applyForce(glm::vec2 force, glm::vec2 pos);
	// Determines what forces will need to be applied when two rigidbodies collide
	void resolveCollision(RigidBody* other, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	// Converts a local position of the object to a world position
	glm::vec2 toWorld(glm::vec2 point);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Sets whether this object is kinematic or not
	void setIsKinematic(bool isKinematic) { m_isKinematic = isKinematic; }
	// Sets the object's position
	void setPosition(glm::vec2 pos) { m_pos = pos; }
	// Sets the object's rotation
	void setRotation(float rot) { m_rot = rot; }
	// Sets the object's positional and angular velocities to 0
	void clearVelocities() { m_velocity = glm::vec2(0, 0); m_angularVelocity = 0; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns whether this object is kinematic or not
	bool getIsKinematic() { return m_isKinematic; }
	// Returns the object's position
	glm::vec2 getPosition() { return m_pos; }
	// Returns the object's rotation
	float getRotation() { return m_rot; }
	// Returns the object's velocity
	glm::vec2 getVelocity() { return m_velocity; }
	// Returns the object's angular velocity
	float getAngularVelocity() { return m_angularVelocity; }
	// Returns the mass of the object - returns INT_MAX if this object is kinematic
	float getMass() { if (m_isKinematic == true) { return (float)INT_MAX; } else { return m_mass; } }
	// Returns the moment of the object - returns INT_MAX if this object is kinematic
	float getMoment() { if (m_isKinematic == true) { return (float)INT_MAX; } else { return m_moment; } }
	// Returns the kinetic energy of the object
	float getKineticEnergy();

	// Returns the local X-axis of the object
	glm::vec2 getLocalX() { return m_localX; }
	// Returns the local Y-axis of the object
	glm::vec2 getLocalY() { return m_localY; }

	// Returns the linear drag of this object
	float getLinearDrag() { return m_linearDrag; }
	// Returns the angular drag of this object
	float getAngularDrag() { return m_angularDrag; }

protected:
	// Does this object ignore forces being applied to it
	bool m_isKinematic = false;

	// The position of the object
	glm::vec2 m_pos;
	// The velocity of the object
	glm::vec2 m_velocity;

	// The rotation of the object
	float m_rot;
	// The angular velocity of the object
	float m_angularVelocity;
	// The total moment of intertia of this object
	float m_moment;

	// The local X-axis of the object
	glm::vec2 m_localX;
	// The local Y-axis of this object
	glm::vec2 m_localY;

	// The mass of this object
	float m_mass;

	// The linear drag of this object
	float m_linearDrag = 0.2f;
	// The angular drag of this object
	float m_angularDrag = 0.2f;
};