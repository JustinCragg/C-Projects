#pragma once
#include "Creature.h"
#include "SMState.h"

// The component which handles the decision making for a food(green)
// The class updates its current state-which determines the state for the next frame
class StateMachine : public Component {
public:
    // The constructor for the class
    StateMachine(SceneObject* parent, SMState* firstState);

    // Executes the currentState
    void update(std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood);

    // Changes m_currentState to newState
    void changeState(SMState* newState);

    // Returns m_currentState
    SMState* getState();

protected:
    // The current state for the state machine
    SMState* m_currentState = nullptr;
};