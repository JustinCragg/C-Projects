#include "StateMachine.h"

/****************************************************************************************************
****************************************************************************************************/
StateMachine::StateMachine(SceneObject* parent, SMState* firstState) : Component(parent) {
    m_componentType = comp::StateMachine;

    firstState->init((Creature*)parent);
    m_currentState = firstState;
}

/****************************************************************************************************
****************************************************************************************************/
void StateMachine::update(std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    m_currentState->update((Creature*)m_parent, this, allPred, allPrey, allFood);
}

/****************************************************************************************************
****************************************************************************************************/
void StateMachine::changeState(SMState* state) {
    m_currentState->exit((Creature*)m_parent);
    state->init((Creature*)m_parent);
    m_currentState = state;
}

/****************************************************************************************************
****************************************************************************************************/
SMState* StateMachine::getState() {
    return m_currentState;
}