#include "DTSeek.h"
#include "Sprite.h"
#include "Game.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
DTSeek::DTSeek(Creature* creature) {
    m_stateType = state::Seek;

    init(creature);
}

/****************************************************************************************************
****************************************************************************************************/
void DTSeek::init(Creature* creature) {
}

/****************************************************************************************************
****************************************************************************************************/
void DTSeek::update(Creature* creature, Creature* target) {
    seekTarget(creature, target);

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range to eat the food
    if (distanceToTarget < moveDistance || creature->m_targetPos == math::Vector3{ INFINITY, INFINITY, INFINITY }) {
        target->m_health -= 10;
        creature->m_hunger += 10;
    }
}

/****************************************************************************************************
****************************************************************************************************/
void DTSeek::exit(Creature* creature) {
    
}

/****************************************************************************************************
****************************************************************************************************/
void DTSeek::seekTarget(Creature* creature, Creature* target) {
    creature->m_maxSpeed = 20;

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = ((Transform*)target->getComponent(comp::Transform))->getWorldTransform()[2];

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}