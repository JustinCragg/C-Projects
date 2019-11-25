#pragma once
#include "SceneObject.h"

// Overaching parent class
// A component contains specific functionality which can be accessed and used by the components owner
class Component {
public:
    // The constructor for the class
    // The input in the sceneobject which owns the component
    Component(SceneObject* parent) {
        m_parent = parent;
    }

    // The type of component this class is
    comp::comp m_componentType;

    // The parent/owner of this component
    SceneObject* m_parent = nullptr;
};