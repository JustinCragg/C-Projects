#pragma once
#include "Behaviour.h"
#include <vector>

// BTComposite (Behaviour Tree Composite) is a class which is used as a parent for all other classes
// which perform logic (eg. AND, OR)
class BTComposite : public Behaviour {
public:
    // The generic function which all children must overide
    // Returns the result of the function and/or the behaviour's children
    // As input takes the creature whose behaviour tree this is
    virtual result::result execute(Creature* creature) = 0;

    // This function adds the inputed behaviour as a new child
    // Adding it at the end of the vector
    void addChild(Behaviour* child);

    // The children of this behaviour
    std::vector<Behaviour*> m_children;
    // This number is used to determine if and where a child returned pending
    int m_pendingIndex = -1;

protected:

};