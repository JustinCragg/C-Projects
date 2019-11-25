#pragma once
#include "Collider.h"
#include "Texture.h"
#include "SceneObject.h"
#include "Renderer2D.h"
#include <Vector2.h>
#include <string>
#include <coreXorShift.h>

class Sprite : public SceneObject {
public:
    // Sets the sprite's hieracy parent
    // Calls init with base values
    Sprite(SceneObject* parent);
    // Sets the sprite's hieracy parent, the absolute path for the texture
    // And optionally the sprite's position and scale
    // Calls init with values provided
    Sprite(SceneObject* parent, const std::string& path, float x = 0, float y = 0, float length = 1, float width = 1);
    // Applies the values provided
    virtual void init(const std::string& path = "", float x = 0, float y = 0, float length = 1, float width = 1);
    // Default destructor
    virtual ~Sprite();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Display
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void setTexture(const std::string& path);
    aie::Texture* getTexture() const;

    void setDisplayed(bool displayed);
    bool getDisplayed() const;

    virtual void draw(aie::Renderer2D* renderer);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Collider
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // The function is blank and should be overwritten by child classes
    virtual void setupCollider();
    // Draws each of the sprite's colliders as simple shapes
    void drawColliders(aie::Renderer2D* renderer);

    // All of the sprite's colliders
    std::vector<Collider*> m_colliders;

protected:
    // The sprite's texture's absolute file path
    std::string m_filePath = "";
    // The sprite's texture
    aie::Texture* m_texture;
    // Whether the sprite should be drawn
    bool m_displayed;

    // Random number generator
    core::XorShift32 m_rand;
};