#include "BTAttack.h"
#include "Prey.h"
#include "Game.h"
#include <Functions.h>
#include <algorithm>

/****************************************************************************************************
****************************************************************************************************/
BTAttack::BTAttack(Creature* creature) {
    m_stateType = state::Attack;
}

/****************************************************************************************************
****************************************************************************************************/
void BTAttack::init(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
result::result BTAttack::update(Creature* creature) {
    Prey* prey = (Prey*)creature;

    seekEnemy(creature);

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)prey->m_preds[0]->getComponent(comp::Transform))->getWorldTransform()[2]);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range to deal damage to the pred
    if (distanceToTarget <= moveDistance && Game::getGameTime() - m_time > 1) {
        m_time = Game::getGameTime();
        prey->m_preds[0]->m_health -= 10;
    }

    return result::Success;
}

/****************************************************************************************************
****************************************************************************************************/
void BTAttack::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTAttack::seekEnemy(Creature* creature) {
    Prey* prey = (Prey*)creature;

    for (Creature* pred : prey->m_preds) {
        if (pred->dead == false) {
            // Determines the distance beween each pred and the prey's home
            pred->m_distance = math::distanceBetweenSq(((Transform*)pred->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_homePos);
        }
    }
    // Sorts the preds by their distance to the home - closest first
    std::sort(((Prey*)creature)->m_preds.begin(), ((Prey*)creature)->m_preds.end(), lessThan());

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = ((Transform*)prey->m_preds[0]->getComponent(comp::Transform))->getWorldTransform()[2];

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}