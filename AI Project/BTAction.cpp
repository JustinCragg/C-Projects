#include "BTAction.h"

#include "BTAttack.h"
#include "BTEvade.h"
#include "BTFlee.h"
#include "BTPatrol.h"
#include "BTSeek.h"
#include "BTWander.h"

/****************************************************************************************************
****************************************************************************************************/
BTAction::BTAction(BTState* state) {
    m_state = state;
}

/****************************************************************************************************
****************************************************************************************************/
result::result BTAction::execute(Creature* creature) {
    switch (m_state->m_stateType) {
    case state::Attack:
        return ((BTAttack*)m_state)->update(creature);
    case state::Evade:
        return ((BTEvade*)m_state)->update(creature);
    case state::Flee:
        return ((BTFlee*)m_state)->update(creature);
    case state::Patrol:
        return ((BTPatrol*)m_state)->update(creature);
    case state::Seek:
        return ((BTSeek*)m_state)->update(creature);
    case state::Wander:
        return ((BTWander*)m_state)->update(creature);
    default:
        return result::Failure;
    }
}