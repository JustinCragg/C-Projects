#pragma once
#include "DTNode.h"

// DTQuestion (Decision Tree Question) is a node which checks the member value and determines which branch to move down
class DTQuestion : public DTNode {
public:
    // Constructor for the class, determines which boolean to be checked
    DTQuestion(bool* value);

    // Checks the boolean then calls the makeDecision function of the corresponding branch
    void makeDecision(Creature* creature);

    // Adds a new node, into either the true or false branch
    // Replaces any branch which may already be there
    void newNode(bool branch, DTNode* newNode);

    // The node which is called if value is true
    DTNode* m_trueBranch;
    // The node which is called if value is false
    DTNode* m_falseBranch;

protected:
    // A reference to the boolean value to be checked
    bool* m_value = nullptr;

};