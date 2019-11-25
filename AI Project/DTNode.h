#pragma once

class Creature;

// Overaching parent class
// DTNode (Decision Tree Node) is the base class for each node making up the decision tree
class DTNode {
public:
    // The generic function which all children must overide
    // Determines what the next action to make will be
    virtual void makeDecision(Creature* creature) = 0;
};