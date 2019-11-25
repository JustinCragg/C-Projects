#include "BTWander.h"
#include "Game.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
BTWander::BTWander(Creature* creature) {
    m_stateType = state::Wander;

    creature->m_targetPos = math::Vector3{ INFINITY, INFINITY, INFINITY };
}

/****************************************************************************************************
****************************************************************************************************/
void BTWander::init(Creature* creature) {
}

/****************************************************************************************************
****************************************************************************************************/
result::result BTWander::update(Creature* creature) {
    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * Game::getDeltaTime() * 25;

    // Determines if the creature has -roughly- moved within range of the target
    if (distanceToTarget < moveDistance || creature->m_targetPos == math::Vector3{ INFINITY, INFINITY, INFINITY }) {
        randGoal(creature);
        m_time = Game::getGameTime();
    }

    // If the prey has not reached the target within 5 game seconds get a new target
    if (Game::getGameTime() - m_time >= 5) {
        randGoal(creature);
        m_time = Game::getGameTime();
    }

    return result::Success;
}

/****************************************************************************************************
****************************************************************************************************/
void BTWander::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTWander::randGoal(Creature* creature) {
    creature->m_maxSpeed = 15;
    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];

    // Random angle
    core::XorShift32& randGen = creature->m_randGen;
    float angle = (float)(randGen.rand() % (int)((2 * 3.14f) * 100));
    angle /= 100;

    // Random angle on the circumference
    float x = m_wandRadius * cos(angle);
    float y = m_wandRadius * sin(angle);
    math::Vector3 direc{ x,y,0 };

    // Random jitter
    x = (float)(randGen.rand() % 200);
    x -= 100;
    x /= 100;
    y = (float)(randGen.rand() % 200);
    y -= 100;
    y /= 100;
    
    math::Vector3 jitter = { x, y, 0 };
    jitter.normalise();
    jitter *= m_wandJitter;

    // Apply jitter
    direc += jitter;

    // Return to circumference
    direc.normalise();
    direc *= m_wandRadius;

    // Move infront of agent
    math::Vector3 vel = creature->m_velocity;
    math::Vector3 zero = { 0,0,0 };
    if (!(creature->m_velocity == zero)) {
        vel.normalise();
    }
    else {
        vel = math::Vector3{ 1,0,0 };
    }
    direc += (vel*m_wandDistance);

    // Bounce off walls
    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = agentPos + direc;
    direc.normalise();
    creature->m_direcForce = direc;
}