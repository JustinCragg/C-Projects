#pragma once
#include "SceneObject.h"

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    // Base class for all resources
    class EXPORT BaseResource : public SceneObject {
    public:
        // Standard constructor
        BaseResource();
        // Standard destructor
        virtual ~BaseResource();

        // Initialises the resource
        void init(glm::vec2 position);
        // Initialises the resource
        // Defaults colour to the script's colour and radius to 15
        void init(glm::vec2 position, glm::vec4 colour, float radius);

        // Determines what needs are important and moves towards them
        // This function is called by the gameManager is used called using multi-threading
        // THIS FUNCTION CANNOT CHANGE VALUES IN OTHER OBJECTS
        virtual void update(float deltaTime);
        // Draws the agent
        virtual void draw(aie::Renderer2D* renderer);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Getters
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Gets the amount of resouces that is left
        float getQuantity() { return m_quantity; }
        // Gets the harvest rate of this resource
        float getHarvestRate() { return m_gathering; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Setters
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Initialises this resource's values according to the template given
        void setScriptValues(ResourceTemplate* resourceTemplate);
        // Updates this resource's values according to the template given
        void updateScriptValues(ResourceTemplate* resourceTemplate);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Misc.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Returns the amount of this resource able to be gathered and removes that amount from quantity
        float consume(float deltaTime);

    protected:
        // The maximum amount of this resource that can be contained
        float m_maxQuantity;
        // The amount of resorce contained
        float m_quantity;

        // The rate at which this resource grows
        float m_regrowth;
        // The rate at which an agent collects this resource
        float m_gathering;

        // The maximum radius the resource will have
        float m_maxRadius;
    };
}