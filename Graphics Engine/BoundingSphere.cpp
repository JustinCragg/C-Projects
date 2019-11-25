#include "BoundingSphere.h"
#include <glm/ext.hpp>

/************************************************************************************************************
************************************************************************************************************/
BoundingSphere::BoundingSphere(std::vector<glm::vec4> positions) {
	fit(positions);
}

/************************************************************************************************************
************************************************************************************************************/
BoundingSphere::BoundingSphere(glm::vec3 centre, float radius) {
	m_centre = centre;
	m_radius = radius;
}

/************************************************************************************************************
************************************************************************************************************/
BoundingSphere::~BoundingSphere() {
}

/************************************************************************************************************
************************************************************************************************************/
void BoundingSphere::fit(std::vector<glm::vec4> positions) {
	glm::vec3 min(INFINITY);
	glm::vec3 max(-INFINITY);

	for (int i = 0; i < positions.size(); i++) {
		glm::vec4 vert = positions[i];
		if (vert.x < min.x) {
			min.x = vert.x;
		}
		if (vert.y < min.y) {
			min.y = vert.y;
		}
		if (vert.z < min.z) {
			min.z = vert.z;
		}
		if (vert.x > max.x) {
			max.x = vert.x;
		}
		if (vert.y > max.y) {
			max.y = vert.y;
		}
		if (vert.z > max.z) {
			max.z = vert.z;
		}
	}

	m_centre = (min + max) * 0.5f;
	m_radius = glm::distance(min, m_centre);
}