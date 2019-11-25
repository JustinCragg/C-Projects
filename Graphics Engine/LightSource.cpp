#include "LightSource.h"
#include <Gizmos.h>

/************************************************************************************************************
************************************************************************************************************/
LightSource::LightSource(glm::vec3 localPos, glm::vec3 diffuse, glm::vec3 specular, LightType type, SceneObject* parent) : SceneObject(localPos, glm::vec3(0), glm::vec3(1), parent) {
	m_direction = m_worldTransform[2];
	m_diffuse = diffuse;
	m_specular = specular;

    m_lightType = type;

    updateTransform();
}

/************************************************************************************************************
************************************************************************************************************/
LightSource::~LightSource() {

}

/************************************************************************************************************
************************************************************************************************************/
void LightSource::update() {
	SceneObject::update();

	m_direction = m_worldTransform[2];
}

/************************************************************************************************************
************************************************************************************************************/
void LightSource::updateGizmos(SceneObject* _camera) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
        switch (m_lightType) {
        case DIRECTIONAL:
            aie::Gizmos::addLine(m_worldTransform[3], m_worldTransform[3] + m_worldTransform[2], glm::vec4(m_diffuse, 1));
            aie::Gizmos::addSphere(m_worldTransform[3], 0.1f, 5, 5, glm::vec4(m_diffuse, 1));
            break;
        case POINT:
            aie::Gizmos::addSphere(m_worldTransform[3], 0.1f, 5, 5, glm::vec4(m_diffuse, 1));
            aie::Gizmos::addSphere(m_worldTransform[3], m_intensity, 5, 5, glm::vec4(m_diffuse, 0.1));
            break;
        case SPOT:
            aie::Gizmos::addLine(m_worldTransform[3], m_worldTransform[3] + m_worldTransform[2], glm::vec4(m_diffuse, 1));
            aie::Gizmos::addSphere(m_worldTransform[3], 0.1f, 5, 5, glm::vec4(m_diffuse, 1));
            break;
        }
		SceneObject::updateGizmos(_camera);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void LightSource::drawMesh(SceneObject* _camera, SceneObject** lightSource) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		SceneObject::drawMesh(_camera, lightSource);
	}
}