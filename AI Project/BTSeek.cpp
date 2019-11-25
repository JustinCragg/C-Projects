#include "BTSeek.h"
#include "Game.h"
#include "Prey.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
BTSeek::BTSeek(Creature* creature) {
    m_stateType = state::Seek;
}

/****************************************************************************************************
****************************************************************************************************/
void BTSeek::init(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
result::result BTSeek::update(Creature* creature) {
    Prey* prey = (Prey*)creature;

    seekTarget(creature);

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range to eat the food
    if (distanceToTarget < moveDistance) {
        prey->m_foods[0]->m_health -= 10;
        creature->m_hunger += 10;
        // Everytime the prey eats but only once every 5 game seconds
        if (Game::getGameTime() - prey->m_eatTimer < 5 || prey->m_eatTimer == 0) {
            // Add two points to the homePoints
            prey->m_eatTimer = Game::getGameTime();
            prey->addHomePoint();
            prey->addHomePoint();
        }
    }

    return result::Success;
}

/****************************************************************************************************
****************************************************************************************************/
void BTSeek::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTSeek::seekTarget(Creature* creature) {
    Prey* prey = (Prey*)creature;

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = ((Transform*)prey->m_foods[0]->getComponent(comp::Transform))->getWorldTransform()[2];

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}