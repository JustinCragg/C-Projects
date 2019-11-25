#pragma once
#include "GameManager.h"

#include <Renderer2D.h>
#include <glm\ext.hpp>

#include <vector>

class SceneObject {
public:
    SceneObject(glm::vec2 position, GameManager::ObjectType type);
    SceneObject(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius = 15);
    virtual ~SceneObject();

    virtual void update();
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
    bool moveTowards(glm::vec2 target);
    // Move the object towards the target, by speed*deltaTime
    // Returns true when the objects has reached the target
    bool moveTowards(float x, float y) { return moveTowards(glm::vec2(x, y)); }

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
    bool lookAt(glm::vec2 target, bool instant = false);
    // Sets the rotation to look at target, if instant is false the object rotates at speed*0.5f*deltaTime
    // Returns true when the object is looking at the target
    bool lookAt(float x, float y, bool instant = false) { return lookAt(glm::vec2(x, y), instant); }

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

    // Returns the type of object this is
    GameManager::ObjectType getObjectType() { return m_objectType; }

    // Returns the colour of the object
    glm::vec4 getColour() { return m_colour; }

	// The distance between this object and another object
	float m_distance;

	void setGameManager(GameManager* gameManager) { m_gameManager = gameManager; }

protected:
    void init(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius);

    GameManager* m_gameManager;

    GameManager::ObjectType m_objectType;

    glm::vec2 m_position = glm::vec2(0);
    float m_rotation = 0;
    float m_radius = 5;

    glm::vec4 m_colour = glm::vec4(1);

    float m_speed = 15;
};