#include "Transform.h"
#include "Sprite.h"
#include <Vector3.h>
#include <Matrix3.h>
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
Transform::Transform(SceneObject* parent, Transform* transformParent) : Component (parent) {
    init(transformParent);
}

/****************************************************************************************************
****************************************************************************************************/
Transform::Transform(SceneObject* parent, Transform* transformParent, float x, float y, float length, float height, float rot) : Component (parent) {
    init(transformParent, x, y, length, height, rot);
}

/****************************************************************************************************
****************************************************************************************************/
void Transform::init(Transform* transformParent, float x, float y, float length, float height, float rot) {
    m_componentType = comp::Transform;

    m_transformParent = transformParent;
    // If the transform has a parent become a child of it
    if (m_transformParent != nullptr) {
        m_transformParent->m_children.push_back(this);
    }

    setPosition(x, y);
    setRotation(rot);
    
    if (length == 0 || height == 0) {
        setScale(1, 1);
    }
    else {
        Sprite* sprite = (Sprite*)(m_parent->getComponent(comp::Sprite));
        if (sprite == nullptr) {
            setScale(length, height);
        }
        else {
            if (sprite->getTexture() == nullptr) {
                setScale(length, height);
            }
            // If sceneObject also has a sprite determine length and width using the sprite size
            else {
                setScale(length / sprite->getTexture()->getWidth(), height / sprite->getTexture()->getHeight());
            }
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
Transform::~Transform() {}

/****************************************************************************************************
****************************************************************************************************/
void Transform::updateLocalTransform() {
    if (m_dirty == true) {
        // Build the local transform from the standard values
        math::Matrix3 translation, rotation, scale;
        translation[2] = m_pos;
        rotation.setRotateZ(m_rot);
        scale.setScale(m_scale);

        m_localTransform = translation * rotation * scale;

        m_dirty = false;
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Transform::updateWorldTransform() {
    if (m_transformParent == nullptr) {
        m_worldTransform = m_localTransform;
    }
    else {
        m_worldTransform = m_transformParent->m_worldTransform * m_localTransform;
    }

    for each (Transform* child in m_children) {
        child->updateTransforms();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Transform::updateTransforms() {
    updateLocalTransform();
    updateWorldTransform();
}

/****************************************************************************************************
****************************************************************************************************/
math::Matrix3 Transform::getLocalTransform() const {
    return m_localTransform;
}

/****************************************************************************************************
***************************************************************************************************/
math::Matrix3 Transform::getWorldTransform() const {
    return m_worldTransform;
}

/****************************************************************************************************
***************************************************************************************************/
math::Vector3 Transform::getPosition() const {
    return m_pos;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::setPosition(float x, float y) {
    m_pos = math::Vector3{ x,y,1 };
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::setPosition(math::Vector3 position) {
    m_pos = position;
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::movePosition(float x, float y) {
    m_pos += math::Vector3{ x,y,1 };
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::movePosition(math::Vector3 offset) {
    m_pos += offset;
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
bool Transform::moveTowards(math::Vector3 target, float step) {
    math::Vector3 move = target -m_pos;
    move.normalise();
    move *= step;

    // If moving will result in reaching the target
    if (math::distanceBetweenSq(move + m_worldTransform[2], target) <= step * step || target == m_worldTransform[2]){
        // Set position
        setPosition(target);
        m_dirty = true;
        // Reached target
        return true;
    }
    else {
        // Move normally
        movePosition(move);
        m_dirty = true;
        // Has not reached target
        return false;
    }
}

/****************************************************************************************************
***************************************************************************************************/
math::Vector3 Transform::getScale() const {
    return m_scale;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::setScale(float x, float y) {
    m_scale = math::Vector3{ x,y,1 };
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::setScale(math::Vector3 scale) {
    m_scale = scale;
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
float Transform::getAngleRotation() const {
    return m_rot;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::setRotation(float rot) {
    m_rot = rot;
    m_dirty = true;
}

/****************************************************************************************************
***************************************************************************************************/
void Transform::rotateRotation(float rot) {
    m_rot += rot;
    m_dirty = true;
}