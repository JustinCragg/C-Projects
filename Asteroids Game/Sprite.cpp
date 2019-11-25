#include "Sprite.h"
#include "Collider.h"
#include <Matrix3.h>

/****************************************************************************************************
****************************************************************************************************/
Sprite::Sprite(SceneObject* parent) : SceneObject(parent) {
    init();
}

/****************************************************************************************************
****************************************************************************************************/
Sprite::Sprite(SceneObject* parent, const std::string& path, float x, float y, float length, float width) : SceneObject(parent) {
    init(path, x, y, length, width);
}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::init(const std::string& path, float x, float y, float width, float height) {
    m_displayed = true;

    // Defaults to 0,0
    setPosition(x, y);

    // Defaults to 1,1
    setScale(width, height);

    setupCollider();

    if (path != "") {
        m_filePath = path;
        setTexture(path);
    }
}

/****************************************************************************************************
****************************************************************************************************/
Sprite::~Sprite() {}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::setTexture(const std::string& path) {
    m_texture = new aie::Texture(path.c_str());
}

/****************************************************************************************************
****************************************************************************************************/
aie::Texture* Sprite::getTexture() const {
    return m_texture;
}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::setDisplayed(bool displayed) {
    m_displayed = displayed;
}

/****************************************************************************************************
****************************************************************************************************/
bool Sprite::getDisplayed() const {
    return m_displayed;
}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::draw(aie::Renderer2D* renderer ) {
    if (m_displayed == true) {
        renderer->drawSpriteTransformed3x3(m_texture, getWorldTransform());
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::setupCollider() {
    // Should be defined in child classes
}

/****************************************************************************************************
****************************************************************************************************/
void Sprite::drawColliders(aie::Renderer2D* renderer) {
    for (int i = 0; i < m_colliders.size(); i++) {
        if (m_colliders[i]->m_type == "box") {
            renderer->drawBox(m_colliders[i]->getWorldTransform()[2].x, m_colliders[i]->getWorldTransform()[2].y, m_colliders[i]->getWorldTransform().getScale().x, m_colliders[i]->getWorldTransform().getScale().y, m_colliders[i]->getWorldTransform().getEuler().z);
        }
        else if (m_colliders[i]->m_type == "circle") {
            renderer->drawCircle(m_colliders[i]->getWorldTransform()[2].x, m_colliders[i]->getWorldTransform()[2].y, m_colliders[i]->getWorldTransform().getScale().x);
        }
    }
}