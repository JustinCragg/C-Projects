#pragma once
#include "DTNode.h"
#include "DTState.h"

// DTAnswer (Decision Tree Answer) is a node which calls the member state
class DTAnswer : public DTNode {
public:
    // Constructor for the class determines which state is called
    DTAnswer(DTState* state);

    // Calls the update function of the state
    void makeDecision(Creature* creature);

protected:
    DTState* m_state = nullptr;
};