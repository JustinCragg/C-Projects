#pragma once
#include "Behaviour.h"

class BTState;

// The BTAction (Behaviour Tree Action) class executes a state when it is called
class BTAction : public Behaviour {
public:
    // Constructor whose determines the state which is called
    BTAction(BTState* state);

    // The overloaded function which executes the member state on the input Creature
    result::result execute(Creature* creature);

protected:
    // The state run by this BTAction
    BTState* m_state = nullptr;
};