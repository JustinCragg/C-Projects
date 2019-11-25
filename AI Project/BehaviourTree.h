#pragma once
#include "Component.h"
#include "Behaviour.h"

// The component which handles the decision making for a prey(blue)
// The class determines how the prey 'feels' and then acts depending on the behaviour tree
class BehaviourTree : public Component {
public:
    // The constructor for the class
    // parent is the sceneObject which owns the component
    BehaviourTree(SceneObject* parent);

    // Determines the values of the creature then executes the first behaviour in the tree
    void update(std::vector<Creature*> allPreds, std::vector<Creature*> allPreys, std::vector<Creature*> allFoods);

protected:
    // Modifies the owner's boolean 'feelings' which determines the decision making
    // Updates the prey's lists of pred, prey and food
    void determineValues(std::vector<Creature*> allPreds, std::vector<Creature*> allPreys, std::vector<Creature*> allFoods);

    // The top/first behaviour in the tree
    Behaviour* m_start = nullptr;
};