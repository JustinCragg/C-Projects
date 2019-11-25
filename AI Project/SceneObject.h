#pragma once

#include <vector>
#include <string>

// Enum containing each of the possible components which can exist
namespace comp {
    enum comp { BehaviourTree, DecisionTree, NavAgent, NavMap, Sprite, StateMachine, Transform };
}

class Component;

// Overarching parent class
// SceneObjects contain components which determine how the sceneObject can act
class SceneObject {
public:
    // Returns the component of the same type as the input
    // If there are no matching components, nullptr is returned
    Component* getComponent(comp::comp type);

    // Adds the input component to the sceneObject's m_components
    void addComponent(Component* component);

    // A vector of all the components the sceneObject has
    std::vector<Component*> m_components;
};