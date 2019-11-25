#pragma once
#include "Behaviour.h"

// The BTCondition (Behaviour Tree Condition) class checks a boolean value to determine which result to return
class BTCondition : public Behaviour {
public:
    // Constructor whose determines the boolean to be checked
    BTCondition(bool* value);

    // The overloaded function which checks the member value and returns the result
    result::result execute(Creature* creature);

protected:
    // The reference to the boolean being checked
    bool* m_value;
};