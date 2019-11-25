#pragma once
#include "BoundingSphere.h"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

// The base class for all sceneobjects
// Handles transform hierachy, and drawing of Gizmos and meshes
class SceneObject {
public:
    // Standard constructor
	SceneObject();
    // Constructor with additional parameters
	SceneObject(glm::vec3 localPos, glm::vec3 localRot, glm::vec3 localScale, SceneObject* parent = nullptr);
    // Standard destructor
	~SceneObject();

    // Handles the updating of this object's children
	virtual void update();
    // Handles the updating of Gizmos for this object's children
	virtual void updateGizmos(SceneObject* _camera);
    // Handles the drawing of meshes for this object's children
    // lightsource is an array of LightSource pointers
    // drawMirror is true for when the objects is drawn onto a seperate buffer, eg. post-processing
	virtual void drawMesh(SceneObject* _camera, SceneObject** lightSource, bool drawMirror = false);

	void addChild(SceneObject* child);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Sets the position of the object in world space
	void setWorldPos(glm::vec3 pos) { m_worldTransform[3] = glm::vec4(pos, 0); }
    // Sets the position of the object relative to its parent
	void setLocalPos(glm::vec3 pos) { m_localPos = pos; }
    // Move the object by an amount relative to its parent
	void moveLocalPos(glm::vec3 delta) { m_localPos += delta; }

    // Sets the rotation of the object relative to its parent
	void setLocalRot(glm::vec3 rot) { m_localRot = rot; }
    // Rotates the object by an amount relative to its parent
	void rotateLocalRot(glm::vec3 delta) { m_localRot += delta; }

    // Sets the scale of the object relative to its parent
	void setLocalScale(glm::vec3 scale) { m_localScale = scale; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the world transform of the object
	glm::mat4 getWorldTransform() const { return m_worldTransform; }

    // Returns the world position of the object
	glm::vec4 getWorldPos() { return m_worldTransform[3]; }
    // Retuns the local position of the obeject
	glm::vec3 getLocalPos() { return m_localPos; }

    // Returns the local rotation of the object
	glm::vec3 getLocalRot() { return m_localRot; }

    // Returns the local scale of the object
	glm::vec3 getLocalScale() { return m_localScale; }

protected:
    // Updates the local and world transforms of the object
	void updateTransform();

    // Function which checks whether the bounding sphere can be seen by the camera
	bool onScreen(SceneObject* _camera, BoundingSphere* boundingSphere);
    // Updates the object's bounding sphere which encompasses all its children
	void updateBoundingChildrenSphere();

    // The local position of the object
	glm::vec3 m_localPos;
    // The local rotation of the object
	glm::vec3 m_localRot;
    // The local scale of the object
	glm::vec3 m_localScale;

    // This object's transform hierachy parent
	SceneObject* m_parent;
    // This object's transform hierachy children
	std::vector<SceneObject*> m_children;

    // This world transform of the object
	glm::mat4 m_worldTransform;

	// If this sphere is not inside the camera view, than there is no need to draw this object or any of its children
	BoundingSphere* m_boundingChildrenSphere = nullptr;
	// If this sphere is not inside the camera view, than there is no need to draw this object
	BoundingSphere* m_boundingSphere = nullptr;
};