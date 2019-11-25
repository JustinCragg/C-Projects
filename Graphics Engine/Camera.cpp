#include "Camera.h"
#include <glm\ext.hpp>

/************************************************************************************************************
************************************************************************************************************/
Camera::Camera(glm::vec3 localPos, SceneObject* parent) : SceneObject(localPos, glm::vec3(0), glm::vec3(1), parent) {
	
}

/************************************************************************************************************
************************************************************************************************************/
Camera::~Camera() {

}

/************************************************************************************************************
************************************************************************************************************/
void Camera::setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up) {
	m_viewTransform = glm::lookAt(from, to, up);
	updateProjectionViewTransform();
}

/************************************************************************************************************
************************************************************************************************************/
void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far) {
	m_projectionTransform = glm::perspective(fieldOfView, aspectRatio, near, far);
	updateProjectionViewTransform();
}

/************************************************************************************************************
************************************************************************************************************/
std::vector<glm::vec4> Camera::getFrustumPlanes() {
	std::vector<glm::vec4> planes = std::vector<glm::vec4>();
	glm::vec4 plane;
	// Right plane
	plane.x = m_projectionViewTransform[0][3] - m_projectionViewTransform[0][0];
	plane.y = m_projectionViewTransform[1][3] - m_projectionViewTransform[1][0];
	plane.z = m_projectionViewTransform[2][3] - m_projectionViewTransform[2][0];
	plane.w = m_projectionViewTransform[3][3] - m_projectionViewTransform[3][0];
	planes.push_back(plane);
	// Left plane
	plane.x = m_projectionViewTransform[0][3] + m_projectionViewTransform[0][0];
	plane.y = m_projectionViewTransform[1][3] + m_projectionViewTransform[1][0];
	plane.z = m_projectionViewTransform[2][3] + m_projectionViewTransform[2][0];
	plane.w = m_projectionViewTransform[3][3] + m_projectionViewTransform[3][0];
	planes.push_back(plane);
	// Top plane
	plane.x = m_projectionViewTransform[0][3] - m_projectionViewTransform[0][1];
	plane.y = m_projectionViewTransform[1][3] - m_projectionViewTransform[1][1];
	plane.z = m_projectionViewTransform[2][3] - m_projectionViewTransform[2][1];
	plane.w = m_projectionViewTransform[3][3] - m_projectionViewTransform[3][1];
	planes.push_back(plane);
	// Bottom plane
	plane.x = m_projectionViewTransform[0][3] + m_projectionViewTransform[0][1];
	plane.y = m_projectionViewTransform[1][3] + m_projectionViewTransform[1][1];
	plane.z = m_projectionViewTransform[2][3] + m_projectionViewTransform[2][1];
	plane.w = m_projectionViewTransform[3][3] + m_projectionViewTransform[3][1];
	planes.push_back(plane);
	// Far plane
	plane.x = m_projectionViewTransform[0][3] - m_projectionViewTransform[0][2];
	plane.y = m_projectionViewTransform[1][3] - m_projectionViewTransform[1][2];
	plane.z = m_projectionViewTransform[2][3] - m_projectionViewTransform[2][2];
	plane.w = m_projectionViewTransform[3][3] - m_projectionViewTransform[3][2];
	planes.push_back(plane);
	// Near plane
	plane.x = m_projectionViewTransform[0][3] + m_projectionViewTransform[0][2];
	plane.y = m_projectionViewTransform[1][3] + m_projectionViewTransform[1][2];
	plane.z = m_projectionViewTransform[2][3] + m_projectionViewTransform[2][2];
	plane.w = m_projectionViewTransform[3][3] + m_projectionViewTransform[3][2];
	planes.push_back(plane);

	for (int i = 0; i < 6; i++) {
		float d = glm::length(glm::vec3(planes[i]));
		planes[i] /= d;
	}

	return planes;
}

/************************************************************************************************************
************************************************************************************************************/
void Camera::updateProjectionViewTransform() {
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}