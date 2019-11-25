#include "BTFlee.h"
#include "Prey.h"
#include "NavMap.h"
#include "NavAgent.h"
#include "Game.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
BTFlee::BTFlee(Creature* creature) {
    m_stateType = state::Flee;
}

/****************************************************************************************************
****************************************************************************************************/
void BTFlee::init(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
result::result BTFlee::update(Creature* creature) {
    NavAgent* navAgent = (NavAgent*)creature->getComponent(comp::NavAgent);

    // Determines when to calculate a new path
    if (navAgent->m_gotPath == false || Game::getGameTime() - ((Prey*)creature)->m_pathingTimer > 5) {
        navAgent->m_navMap->resetNodes();
        navAgent->calculatePath(creature->m_homePos, ((Prey*)creature)->m_preds);
        ((Prey*)creature)->m_pathingTimer = Game::getGameTime();
    }

    // Moves towards the next node in the path
    seekNode(creature);
    if (((Prey*)creature)->m_preds.empty() == false) {
        if (((Prey*)creature)->m_preds[0]->m_distance <= SHORT_DISTANCE * SHORT_DISTANCE) {
            // Pushes away from the closest pred if there is one in range
            evadeEnemy(creature);
        }
    }
    
    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range of the next node
    if (distanceToTarget < moveDistance) {
        navAgent->m_nodePath.pop_back();
        // When the path is empty
        if (navAgent->m_nodePath.empty() == true) {
            navAgent->m_gotPath = false;
            // The Flee succeeds
            return result::Success;
        }
    }

    // The creatures continues fleeing, regardless of other factors, until it reaches its goal
    return result::Pending;
}

/****************************************************************************************************
****************************************************************************************************/
void BTFlee::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTFlee::seekNode(Creature* creature) {
    NavAgent* navAgent = (NavAgent*)creature->getComponent(comp::NavAgent);

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = navAgent->m_nodePath.back()->pos;

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}

/****************************************************************************************************
****************************************************************************************************/
void BTFlee::evadeEnemy(Creature* creature) {
    Prey* prey = (Prey*)creature;
    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 direc;

    direc = (agentPos - ((Transform*)prey->m_preds[0]->getComponent(comp::Transform))->getWorldTransform()[2]);

    // The push is minimised to only slightly impact the overal movement
    direc *= 0.1f;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos += direc/2;
    direc.normalise();
    creature->m_direcForce += direc;
}