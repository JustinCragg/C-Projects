#include "stdafx.h"
#include "BaseResource.h"

namespace flai {
    /************************************************************************************************************
    ************************************************************************************************************/
    BaseResource::BaseResource() {
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    BaseResource::~BaseResource() {

    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::init(glm::vec2 position) {
        m_position = position;
        m_maxRadius = m_radius;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::init(glm::vec2 position, glm::vec4 colour, float radius) {
        m_position = position;
        m_colour = colour;
        m_radius = radius;
        m_maxRadius = radius;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::setScriptValues(ResourceTemplate* resourceTemplate) {
        m_objectType = resourceTemplate->type;
        m_colour = resourceTemplate->baseColour;
        m_maxQuantity = resourceTemplate->baseQuantity;
        m_quantity = resourceTemplate->baseQuantity;
        m_regrowth = resourceTemplate->baseRegrowth;
        m_gathering = resourceTemplate->baseGatherAmount;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::updateScriptValues(ResourceTemplate* resourceTemplate) {
        m_colour = resourceTemplate->baseColour;
        m_maxQuantity = resourceTemplate->baseQuantity;
        m_regrowth = resourceTemplate->baseRegrowth;
        m_gathering = resourceTemplate->baseGatherAmount;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::update(float deltaTime) {
        SceneObject::update(deltaTime);

        if (m_quantity < m_maxQuantity) {
            m_quantity += m_regrowth * deltaTime;
        }
        if (m_quantity == INFINITY || m_maxQuantity == INFINITY) {
            m_radius = m_maxRadius;
        }
        else {
            m_radius = m_quantity / m_maxQuantity * m_maxRadius;
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseResource::draw(aie::Renderer2D* renderer) {
        SceneObject::draw(renderer);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float BaseResource::consume(float deltaTime) {
        if (m_quantity - m_gathering * deltaTime < 0) {
            float temp = 0;
            temp = m_quantity;
            m_quantity = 0;
            return temp;
        }
        else {
            m_quantity -= m_gathering * deltaTime;
            return m_gathering * deltaTime;
        }
    }
}