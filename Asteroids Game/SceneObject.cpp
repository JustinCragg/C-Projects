#include "SceneObject.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
SceneObject::SceneObject(SceneObject* parent) {
    if (parent != nullptr) {
        m_parent = parent;
        parent->m_children.push_back(this);
    }
}

/****************************************************************************************************
****************************************************************************************************/
SceneObject::~SceneObject() {}

/****************************************************************************************************
****************************************************************************************************/
SceneObject* SceneObject::getWorldRoot() {
    if (m_parent == nullptr) {
        return this;
    }
    else {
        return m_parent->getWorldRoot();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::updateWorldTransform() {
    if (m_parent == nullptr) {
        m_worldTransform = m_localTransform;
    }
    else {
        m_worldTransform = m_parent->m_worldTransform * m_localTransform;
    }

    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->update();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::updateLocalTransform() {
    if (m_dirty == true) {
        math::Matrix3 translation, rotation, scale;
        translation[2] = m_state.pos;
        rotation.setRotateZ(m_state.rot);
        scale.setScale(m_state.scale);

        m_localTransform = translation * rotation * scale;

        m_dirty = false;
    }
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::update() {
    updateLocalTransform();
    updateWorldTransform();
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::draw() {
    // Should be determined by child classes
}

/****************************************************************************************************
****************************************************************************************************/
math::Matrix3 SceneObject::getLocalTransform() const {
    return m_localTransform;
}

/****************************************************************************************************
****************************************************************************************************/
math::Matrix3 SceneObject::getWorldTransform() const {
    return m_worldTransform;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setLocalTransform(math::Matrix3 newLocalTransform) {
    m_localTransform = newLocalTransform;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setPosition(float x, float y, float z) {
    m_state.pos = math::Vector3{ x,y,z };
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setPosition(math::Vector3 position) {
    m_state.pos = position;
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::movePosition(float x, float y, float z) {
    m_state.pos += math::Vector3{ x,y,z };
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::movePosition(math::Vector3 position) {
    m_state.pos += position;
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
math::Vector3 SceneObject::getPosition() const {
    return m_state.pos;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setRotation(float angle) {
    m_state.rot = angle;
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::rotateRotation(float angle) {
    m_state.rot += angle;
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
float SceneObject::getAngleRotation() const {
    return m_state.rot;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setScale(float x, float y, float z) {
    m_state.scale = math::Vector3{ x, y, z};
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setScale(math::Vector3 scale) {
    m_state.scale = scale;
    m_dirty = true;
}

/****************************************************************************************************
****************************************************************************************************/
math::Vector3 SceneObject::getScale() {
    return m_state.scale;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::setParent(SceneObject* newParent) {
    std::vector<SceneObject*> vec = m_parent->m_children;
    int index = (int)(std::find(vec.begin(), vec.end(), this) - vec.begin());
    vec.erase(vec.begin() + index);

    newParent->m_children.push_back(this);
    m_parent = newParent;
}