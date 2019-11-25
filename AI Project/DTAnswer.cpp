#include "DTAnswer.h"
#include "DTEvade.h"
#include "DTSeek.h"
#include "DTWander.h"

#include "Predator.h"

/****************************************************************************************************
****************************************************************************************************/
DTAnswer::DTAnswer(DTState* state) {
    m_state = state;
}

/****************************************************************************************************
****************************************************************************************************/
void DTAnswer::makeDecision(Creature* creature) {
    switch (m_state->m_stateType) {
    case state::Evade:
        ((DTEvade*)m_state)->update(creature, ((Predator*)creature)->m_preys);
        break;
    case state::Seek:
        ((DTSeek*)m_state)->update(creature, ((Predator*)creature)->m_preys[0]);
        break;
    case state::Wander:
        ((DTWander*)m_state)->update(creature);
        break;
    }
}