#include "BTPatrol.h"
#include "Prey.h"
#include "Game.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
BTPatrol::BTPatrol(Creature* creature) {
    m_stateType = state::Patrol;
    init(creature);
}

/****************************************************************************************************
****************************************************************************************************/
void BTPatrol::init(Creature* creature) {
    // A starting angle is randomly determined
    m_angle = (float)((int)creature->m_randGen.rand() % (2 * 314));
    m_angle /= 100;
}

/****************************************************************************************************
****************************************************************************************************/
result::result BTPatrol::update(Creature* creature) {
    Prey* prey = (Prey*)creature;

    seekTarget(creature);

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range of the target
    if (distanceToTarget < moveDistance) {
        m_angle += 0.5f;
    }

    return result::Success;
}

/****************************************************************************************************
****************************************************************************************************/
void BTPatrol::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTPatrol::seekTarget(Creature* creature) {
    Prey* prey = (Prey*)creature;

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = math::Vector3{ SHORT_DISTANCE/2*cos(m_angle), SHORT_DISTANCE/2*sin(m_angle), 1 };
    targetPos += creature->m_homePos;

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}