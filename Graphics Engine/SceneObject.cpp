#include "SceneObject.h"
#include "Application3D.h"
#include "Camera.h"

#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\quaternion.hpp>

#include <iostream>

/************************************************************************************************************
************************************************************************************************************/
SceneObject::SceneObject() {
	m_localPos = glm::vec3(0);
	m_localRot = glm::vec3(0);
	m_localScale = glm::vec3(1);

	m_parent = nullptr;
	updateTransform();
}

/************************************************************************************************************
************************************************************************************************************/
SceneObject::SceneObject(glm::vec3 localPos, glm::vec3 localRot, glm::vec3 localScale, SceneObject* parent) {
	m_localPos = localPos;
	m_localRot = localRot;
	m_localScale = localScale;

	m_parent = parent;
	if (m_parent != nullptr) {
		m_parent->addChild(this);
	}
	updateTransform();
}

/************************************************************************************************************
************************************************************************************************************/
SceneObject::~SceneObject() {
    for each (SceneObject* child in m_children) {
        delete child;
    }
    delete m_boundingChildrenSphere;
    delete m_boundingSphere;
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::update() {
	for each (SceneObject* child in m_children) {
		child->update();
	}

	updateTransform();
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::updateTransform() {
    // Converts local rotation to a quaternion
	glm::quat rotQuaternion = glm::quat(m_localRot);
    // Rotation matrix
	glm::mat4 rotation = glm::mat4(rotQuaternion);

    // Translation matrix
	glm::mat4 translation = glm::translate(glm::mat4(1), m_localPos);

    // Scale matrix
	glm::mat4 scale = glm::scale(glm::mat4(1), m_localScale);

    // The local transform of the object
	glm::mat4 localTransform = translation * rotation * scale;

	if (m_parent == nullptr) {
		m_worldTransform = localTransform;
	}
	else {
		m_worldTransform = m_parent->getWorldTransform() * localTransform;
	}
}

/************************************************************************************************************
************************************************************************************************************/
bool SceneObject::onScreen(SceneObject* _camera, BoundingSphere* boundingSphere) {
	Camera* camera = dynamic_cast<Camera*>(_camera);

	if (camera != nullptr) {
		if (boundingSphere != nullptr) {
			std::vector<glm::vec4> planes = camera->getFrustumPlanes();

			for (int i = 0; i < 6; i++) {
				float d = glm::dot(glm::vec3(planes[i]), glm::vec3(m_worldTransform[3]) + boundingSphere->getLocalCentre()) + planes[i].w;

				if (d < -boundingSphere->getRadius()) {
					return false;
				}
			}
		}
	}
	else {
		return false;
	}
	return true;
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::updateGizmos(SceneObject* _camera) {
	for each (SceneObject* child in m_children) {
		child->updateGizmos(_camera);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::drawMesh(SceneObject * _camera, SceneObject** lightSource, bool drawMirror) {
	for each (SceneObject* child in m_children) {
		child->drawMesh(_camera, lightSource, drawMirror);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::updateBoundingChildrenSphere() {
	glm::vec3 centre(0);
	float radius = INFINITY;

	for each (SceneObject* child in m_children) {
		child->updateBoundingChildrenSphere();

		if (child->m_boundingChildrenSphere == nullptr) {
			if (child->m_boundingSphere != nullptr) {
				centre += child->m_localPos + child->m_boundingSphere->getLocalCentre();
				if (child->m_boundingSphere->getRadius() > radius) {
					radius = child->m_boundingSphere->getRadius();
				}
			}
		}
		else {
			centre += child->m_localPos + child->m_boundingChildrenSphere->getLocalCentre();
			if (child->m_boundingChildrenSphere->getRadius() > radius) {
				radius = child->m_boundingChildrenSphere->getRadius();
			}
		}
	}

	if (m_children.size() > 0) {
		if (m_boundingChildrenSphere != nullptr) {
			delete m_boundingChildrenSphere;
		}
		m_boundingChildrenSphere = new BoundingSphere(centre, radius);
	}
	else {
		if (m_boundingChildrenSphere != nullptr) {
			delete m_boundingChildrenSphere;
		}
	}
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::addChild(SceneObject* child) {
	if (std::find(m_children.begin(), m_children.end(), child) == m_children.end()) {
		// Not already a child
		m_children.push_back(child);

		updateBoundingChildrenSphere();
	}
}