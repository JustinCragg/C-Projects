#pragma once
#include <Vector3.h>

// Checks if moving in the direction will result in moving of the screen and if so modifies the direction
math::Vector3 screenBounce(math::Vector3 origin, math::Vector3 direc);

// Enum listing the possible states which can be executed
namespace state {
    enum state{ Evade, None, Seek, Wander };
}

// Overarching parent class
// DTState (Decision Tree State) is the base stae which other states inherit from
class DTState {
public:
    // The type of state the state is
    state::state m_stateType = state::None;

protected:
    // Used to store the game time when certain actions are carried out in the states
    float m_time = 0;
};