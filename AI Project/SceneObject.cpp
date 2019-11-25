#include "SceneObject.h"
#include "Component.h"

/****************************************************************************************************
****************************************************************************************************/
Component* SceneObject::getComponent(comp::comp type) {
    for (Component* component : m_components) {
        if (component->m_componentType == type) {
            return component;
        }
    }
    return nullptr;
}

/****************************************************************************************************
****************************************************************************************************/
void SceneObject::addComponent(Component* component) {
    m_components.push_back(component);
}