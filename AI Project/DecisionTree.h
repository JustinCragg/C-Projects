#pragma once
#include "Component.h"
#include "DTNode.h"

// The component which handles the decision making for a pred(red)
// The class determines how the pred 'feels' and then acts depending on the decision tree
class DecisionTree : public Component {
public:
    // The constructor for the class
    // parent is the sceneObject which owns the component
    DecisionTree(SceneObject* parent);

    // Determines the values of the creature then executes the first node in the tree
    void update(std::vector<Creature*> allPreys);

protected:
    // Modifies the owner's boolean 'feelings' which determines the decision making
    void determineValues(std::vector<Creature*> allPreys);

    // The top/first node in the tree
    DTNode* m_startNode = nullptr;
};