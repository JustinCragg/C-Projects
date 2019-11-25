#pragma once
#include "SceneObject.h"

// A Base camera class which inherits from SceneObject
class Camera : public SceneObject {
public:
    // Standard constructor
	Camera(glm::vec3 localPos, SceneObject* parent);
    // The standard destructor
	~Camera();

    // Forces child classes to implement this function
	virtual void update() = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Orients the view transform then updates the projection view matrix
	void setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
    // Sets the projection matrix then updates the projection view matrix
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the view transform of the camera
	glm::mat4 getView() { return m_viewTransform; }
    // Returns the prokjection transform of the camera
	glm::mat4 getProjection() { return m_projectionTransform; }
    // Returns the projection view transform transform of the camera
	glm::mat4 getProjectionView() { return m_projectionViewTransform; }
    // Returns the planes which describe the bounds of the camera
	std::vector<glm::vec4> getFrustumPlanes();

protected:
    // Updates the projection view matrix
	void updateProjectionViewTransform();

    // The view matrix of the camera
	glm::mat4 m_viewTransform;
    // The projection matrix of the camera
	glm::mat4 m_projectionTransform;
    // The projection view matrix of the camera
	glm::mat4 m_projectionViewTransform;
};