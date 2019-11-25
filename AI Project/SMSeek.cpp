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
SMSeek::SMSeek() : SMState::SMState() {
    m_stateType = state::Seek;
}

/****************************************************************************************************
****************************************************************************************************/
void SMSeek::init(Creature* creature) {
}

/****************************************************************************************************
****************************************************************************************************/
void SMSeek::determineNextState(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    // ***** DETERMINE NEXT STATE *****
    for (Creature* food : allFood) {
        if (food->dead == false) {
            // Calculate distance between each prey and the food
            food->m_distance = math::distanceBetweenSq(((Transform*)food->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2]);
        }
    }
    // Sort prey by distance to food
    std::sort(allFood.begin(), allFood.end(), lessThan());
    m_target = allFood[1];

    // The first in the allFood vector should always be the creature doing the searching
    if (!(allFood[1]->m_distance > 25*25)) {
        // Seek (closest food)
        sm->changeState(new SMWander);
    }
    // ***** DETERMINE NEXT STATE FINISH *****
}

/****************************************************************************************************
****************************************************************************************************/
void SMSeek::update(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    determineNextState(creature, sm, allPred, allPrey, allFood);

    // Seek
    seekTarget(creature, m_target);
    pushHome(creature);
}

/****************************************************************************************************
****************************************************************************************************/
void SMSeek::exit(Creature* creature) {
}

/****************************************************************************************************
****************************************************************************************************/
void SMSeek::seekTarget(Creature* creature, Creature* target) {
    creature->m_maxSpeed = 5;

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = ((Transform*)target->getComponent(comp::Transform))->getWorldTransform()[2];

    math::Vector3 direc = targetPos - agentPos;

    direc = screenBounce(agentPos, direc);
    creature->m_targetPos = targetPos;
    direc.normalise();
    creature->m_direcForce = direc;
}