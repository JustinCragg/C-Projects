#pragma once
#include <glm\ext.hpp>

// The different types of objects
// Used for identification and determining which collision calculations to use
enum ShapeType { JOINT = -2, SOFTBODY, PLANE, SPHERE, BOX, SHAPE_COUNT };

// The base class for all object which are implemented using physics
class PhysicsObject {
public:
	// Used for applying physics effects with a stable timeStep
	// Must be overridden
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	// Used to debug through and instance of an object
	// Must be overridden
	virtual void debug() = 0;
	// Assigns the object to be drawn
	// Must be overridden
	virtual void makeGizmo() = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Returns the type of shape the object is
	virtual ShapeType getShapeType() { return m_shapeID; }
	// Returns the elasticity of the object
	virtual float getElasticity() { return m_elasticity; }

	// Returns whether this object destroys other objects it touches
	bool getDestroyer() { return m_destroyer; }
	// Returns whether this object will be removed at the end of this frame
	bool getMarkForRemove() { return m_markForRemove; }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setters
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Sets whether this object will destory objects it collides with
	void setDestroyer(bool destroyer) { m_destroyer = destroyer; }
	// Sets whether this object will need to be removed at the end of the frame
	void setMarkForRemove(bool markForRemove) { m_markForRemove = markForRemove; }

protected:
	// The standard constructor
	PhysicsObject(ShapeType shapeID, float elasticity) { m_shapeID = shapeID; m_elasticity = elasticity; }

	// The type of shape this object is
	ShapeType m_shapeID;

	// The elasticty of this object - how much energy is lost or gained when the object collides
	float m_elasticity;

	// Whether this object destroys other objects it collides with
	bool m_destroyer = false;
	// Whether the obejct will be destroyed at the end of this frame
	bool m_markForRemove = false;
};