#pragma once
#include "Component.h"
#include "Transform.h"

#include <Texture.h>
#include <Renderer2D.h>

// The component which handles the values and function needed for sprites
// The class stores the texture used, and handles it's drawing
class Sprite : public Component {
public:
    // Sets the sprite's transform hieracy parent, the absolute path for the texture
    // And optionally the sprite's position and scale
    // Calls init with values provided
    Sprite(SceneObject* parent, const std::string& path);
    // Applies the values provided
    virtual void init(const std::string& path = "");
    // Default destructor
    virtual ~Sprite();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Display
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Sets the sprite's texture, path is absolute
    void setTexture(const std::string& path);
    // Returns the sprite's texture
    aie::Texture* getTexture() const;

    // Draws the sprite to the renderer
    virtual void draw(aie::Renderer2D* renderer);

    // Wether the sprite should be displayed on the screen
    bool m_display;

protected:
    // The path to the textures file
    std::string m_filePath;

    // The texture used by the sprite
    aie::Texture* m_texture = nullptr;
};