#pragma once
#include "Behaviour.h"

// Checks if moving in the direction will result in moving of the screen and if so modifies the direction by flipping the offending axis
math::Vector3 screenBounce(math::Vector3 origin, math::Vector3 direc);

// Enum listing the possible states which can be be executed
namespace state {
    enum state { Attack, Evade, Flee, None, Patrol, Seek, Wander };
}

// Overarching parent class
// BTState (Behaviour Tree State) is the base state which other states inherit from
class BTState {
public:
    // The generic function which all children must overide
    // Used for initialising a state
    virtual void init(Creature* creature) = 0;
    // The generic function which all children must overide
    // Used for updating the input creature according to the pertaining state
    virtual result::result update(Creature* creature) = 0;
    // The generic function which all children must overide
    // Used for deinitialising a state when it is executed
    virtual void exit(Creature* creature) = 0;

    // The type of state created
    state::state m_stateType = state::None;
protected:
    // Used to store the game time when certain actions are carried out in the states
    float m_time = 0;
};