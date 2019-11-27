#pragma once
#include "GameManager.h"

#include <Renderer2D.h>
#include <glm\ext.hpp>

#include <vector>

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    // Base class for all objects which are used in the scene
    // Handles movement and drawing
    class EXPORT SceneObject {
    public:
        // Standard constructor
        SceneObject() {}
        // Defaults colour to that of the script and radius to 15
        SceneObject(glm::vec2 position, std::string type);
        // Defaults radius to 15
        SceneObject(glm::vec2 position, std::string type, glm::vec4 colour, float radius = 15);
        // Standard Destructor
        virtual ~SceneObject();

        // Handles the updating of the object
        // Needs to be overridden
        virtual void update(float deltaTime);
        // Handles the drawing of drawing of the object
        virtual void draw(aie::Renderer2D* renderer);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Position
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Sets the position of the object to position
        void setPosition(glm::vec2 position) { m_position = position; }
        // Sets the position of the object to position
        void setPosition(float x, float y) { m_position = glm::vec2(x, y); }

        // Moves the object by the delta
        void movePosition(glm::vec2 delta) { m_position += delta; }
        // Moves the object by the delta
        void movePosition(float x, float y) { m_position += glm::vec2(x, y); }

        // Move the object towards the target, by speed*deltaTime
        // Returns true when the objects has reached the target
        bool moveTowards(glm::vec2 target, float deltaTime);
        // Move the object towards the target, by speed*deltaTime
        // Returns true when the objects has reached the target
        bool moveTowards(float x, float y, float deltaTime) { return moveTowards(glm::vec2(x, y), deltaTime); }

        // Returns the position of the object
        glm::vec2 getPosition() { return m_position; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Rotation
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Sets the rotation of the object to rot
        void setRotation(float rot) { m_rotation = rot; }

        // Rotates the rotation of the object by delta
        void rotateRotation(float delta) { m_rotation += delta; }

        // Sets the rotation to look at target, if instant is false the object rotates at speed*0.5f*deltaTime
        // Returns true when the object is looking at the target
        bool lookAt(glm::vec2 target);
        // Sets the rotation to look at target, if instant is false the object rotates at speed*0.5f*deltaTime
        // Returns true when the object is looking at the target
        bool lookAt(float x, float y) { return lookAt(glm::vec2(x, y)); }
        // Sets the rotation to look at target, if instant is false the object rotates at speed*0.5f*deltaTime
        // Returns true when the object is looking at the target
        bool lookAt(glm::vec2 target, float deltaTime);
        // Sets the rotation to look at target, if instant is false the object rotates at speed*0.5f*deltaTime
        // Returns true when the object is looking at the target
        bool lookAt(float x, float y, float deltaTime) { return lookAt(glm::vec2(x, y), deltaTime); }

        // Returns the rotation of the object
        float getRotation() { return m_rotation; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Radius
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Sets the radius of the object
        void setRadius(float radius) { m_radius = radius; }

        // Changes the radius of the object by delta
        void changeRadius(float delta) { m_radius += delta; }

        // Returns the radius of the object
        float getRadius() { return m_radius; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Misc.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Returns the colour of the object
        glm::vec4 getColour() { return m_colour; }

        // Returns the type of object this is
        std::string getType() { return m_objectType; }

        // Gives this objects a reference to the gameManager
        void setGameManager(GameManager* gameManager) { m_gameManager = gameManager; }

    protected:
        // Initialises variables
        void init(glm::vec2 position, std::string type, glm::vec4 colour, float radius);

        // The gameManager which this object is contained in
        GameManager* m_gameManager;

        // The type of object this is
        std::string m_objectType;

        // The object's position
        glm::vec2 m_position = glm::vec2(0);
        // The object's rotation
        float m_rotation = 0;
        // The object's radius
        float m_radius = 5;

        // The object's colour
        glm::vec4 m_colour = glm::vec4(1);

        // The speed the object moves at
        float m_speed = 25;
    };
}