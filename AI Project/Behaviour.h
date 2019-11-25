#pragma once
#include "Creature.h"

// Enum listing the results which can be returned by the behaviours
namespace result {
    enum result { Success, Failure, Pending };
}

// Overarching parent class
// Behaviours dictate how the behaviour tree acts and is laid out
class Behaviour {
public:
    // The generic function which all children must overide
    // Returns the result of the function and/or the behaviour's children
    // As input takes the creature whose behaviour tree this is
    virtual result::result execute(Creature* creature) = 0;

protected:

};