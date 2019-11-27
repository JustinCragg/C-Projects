#include "stdafx.h"
#include "SceneObject.h"

#include <math.h>

namespace flai {
    /************************************************************************************************************
    ************************************************************************************************************/
    SceneObject::SceneObject(glm::vec2 position, std::string type) {
        init(position, type, glm::vec4(1), 15);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    SceneObject::SceneObject(glm::vec2 position, std::string type, glm::vec4 colour, float radius) {
        init(position, type, colour, radius);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    SceneObject::~SceneObject() {

    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void SceneObject::init(glm::vec2 position, std::string type, glm::vec4 colour, float radius) {
        m_position = position;
        m_objectType = type;
        m_colour = colour;
        m_radius = radius;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void SceneObject::update(float deltaTime) {

    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void SceneObject::draw(aie::Renderer2D* renderer) {
        renderer->setRenderColour(m_colour.r, m_colour.g, m_colour.b, m_colour.a);
        renderer->drawCircle(m_position.x, m_position.y, m_radius);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    bool SceneObject::moveTowards(glm::vec2 target, float deltaTime) {
        if (glm::distance(m_position, target) <= m_speed * deltaTime) {
            setPosition(target);
            return true;
        }
        else {
            movePosition(glm::normalize(target - m_position) * m_speed * deltaTime);
            return false;
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    bool SceneObject::lookAt(glm::vec2 target) {
        glm::vec2 direc = glm::normalize(target - m_position);

        float targetRot = std::atan2(direc.y, direc.x);

        m_rotation = targetRot;
        return true;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    bool SceneObject::lookAt(glm::vec2 target, float deltaTime) {
        glm::vec2 direc = glm::normalize(target - m_position);

        float targetRot = std::atan2(direc.y, direc.x);
        float step = 2.0f * glm::pi<float>() * 0.1f * deltaTime;

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