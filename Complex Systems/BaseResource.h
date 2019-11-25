#pragma once
#include "SceneObject.h"

class BaseResource : public SceneObject {
public:
    BaseResource(glm::vec2 position, GameManager::ObjectType type);
    BaseResource(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius = 15);
    virtual ~BaseResource();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gets the amount of resouces that is left
    float getQuantity() { return m_quantity; }
    // Gets the abundance of this resource
    float getAbundance() { return m_abundance; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Misc.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


    // Returns the amount of this resource able to be gathered and removes that amount from quantity
    float consume();

protected:
    virtual float determineAbundance() = 0;

    float m_quantity;
    float m_abundance; // distance, quantity, regrowth of nearby resources of the same type

    float m_regrowth;
    float m_gathering;
};