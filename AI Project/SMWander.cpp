#include "SMWander.h"
#include "SMEvade.h"
#include "SMSeek.h"

#include "Creature.h"
#include "Transform.h"
#include "Game.h"
#include <coreXorShift.h>
#include <Functions.h>
#include <algorithm>

/****************************************************************************************************
****************************************************************************************************/
SMWander::SMWander() : SMState::SMState() {
	m_stateType = state::Wander;
}

/****************************************************************************************************
****************************************************************************************************/
void SMWander::init(Creature* creature) {
    creature->m_targetPos = math::Vector3{ INFINITY, INFINITY, INFINITY };
}

/****************************************************************************************************
****************************************************************************************************/
void SMWander::determineNextState(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    // ***** DETERMINE NEXT STATE *****
    std::vector<Creature*> allCreature;
    allCreature.insert(allCreature.end(), allPred.begin(), allPred.end());
    allCreature.insert(allCreature.end(), allPrey.begin(), allPrey.end());

    for (Creature* bob : allCreature) {
        if (bob->dead == false) {
            // Calculate distance between each creature
            bob->m_distance = math::distanceBetweenSq(((Transform*)bob->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2]);
        }
    }
    // Sort creatures by their distance
    std::sort(allCreature.begin(), allCreature.end(), lessThan());

    for (Creature* food : allFood) {
        if (food->dead == false) {
            // Calculate distance between each prey and the food
            food->m_distance = math::distanceBetweenSq(((Transform*)food->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2]);
        }
    }
    // Sort prey by distance to food
    std::sort(allFood.begin(), allFood.end(), lessThan());

    // Is there a prey or predator nearby?
    if (allCreature.size() > 0) {
        if (allCreature[0]->m_distance < SHORT_DISTANCE*SHORT_DISTANCE) {
            // Evade
            sm->changeState(new SMEvade);
        }
    }
    // The first in the allFood vector should always be the creature doing the searching
    if (allFood.size() > 1) {
        if (allFood[1]->m_distance > 25*25) {
            // Seek (closest food)
            sm->changeState(new SMSeek);
        }
    }
    // ***** DETERMINE NEXT STATE FINISH *****
}

/****************************************************************************************************
****************************************************************************************************/
void SMWander::update(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {    
    determineNextState(creature, sm, allPred, allPrey, allFood);

    // Wander
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

    pushHome(creature);
}

/****************************************************************************************************
****************************************************************************************************/
void SMWander::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void SMWander::randGoal(Creature* creature) {
    creature->m_maxSpeed = 5;
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