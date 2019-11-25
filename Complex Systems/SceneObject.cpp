#include "SceneObject.h"
#include "ComplexSystemsApp.h"

#include <math.h>

/************************************************************************************************************
************************************************************************************************************/
SceneObject::SceneObject(glm::vec2 position, GameManager::ObjectType type) {
    init(position, type, glm::vec4(1), 15);
}

/************************************************************************************************************
************************************************************************************************************/
SceneObject::SceneObject(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius) {
    init(position, type, colour, radius);
}

/************************************************************************************************************
************************************************************************************************************/
SceneObject::~SceneObject() {

}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::init(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius) {
    m_position = position;
    m_objectType = type;
    m_colour = colour;
    m_radius = radius;
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::update() {
    
}

/************************************************************************************************************
************************************************************************************************************/
void SceneObject::draw(aie::Renderer2D* renderer) {
    renderer->setRenderColour(m_colour.r, m_colour.g, m_colour.b, m_colour.a);
    renderer->drawCircle(m_position.x, m_position.y, m_radius);
    //renderer->drawBox(m_position.x, m_position.y, m_radius, m_radius, m_rotation);
}

/************************************************************************************************************
************************************************************************************************************/
bool SceneObject::moveTowards(glm::vec2 target) {
    if (glm::distance(m_position, target) <= m_speed * ComplexSystemsApp::getDeltaTime()) {
        setPosition(target);
        return true;
    }
    else {
        movePosition(glm::normalize(target - m_position) * m_speed * ComplexSystemsApp::getDeltaTime());
        return false;
    }
}

/************************************************************************************************************
************************************************************************************************************/
bool SceneObject::lookAt(glm::vec2 target, bool instant) {
    glm::vec2 direc = glm::normalize(target - m_position);

    float targetRot = std::atan2(direc.y, direc.x);
    float step = 2.0f * glm::pi<float>() * 0.1f * ComplexSystemsApp::getDeltaTime();

    if (instant == true) {
        m_rotation = targetRot;
        return true;
    }
    else {
        float current = std::fmod(m_rotation, 2.0f*glm::pi<float>());
        float totalDelta = targetRot - current;
        if (abs(totalDelta) <= step) {
            m_rotation = targetRot;
            return true;
        }
        else {
            float sign = (totalDelta < 0) ? -1.0f : ((totalDelta > 0) ? 1.0f : 0.0f);
            m_rotation += step * sign;
            return false;
        }
    }
}