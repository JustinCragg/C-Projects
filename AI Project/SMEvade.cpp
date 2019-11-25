#include "SMWander.h"
#include "SMEvade.h"
#include "SMSeek.h"

#include "Creature.h"
#include "Transform.h"
#include <coreXorShift.h>
#include <Functions.h>
#include <algorithm>

/****************************************************************************************************
****************************************************************************************************/
SMEvade::SMEvade() : SMState::SMState() {
    m_stateType = state::Evade;
}

/****************************************************************************************************
****************************************************************************************************/
void SMEvade::init(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void SMEvade::determineNextState(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    // ***** DETERMINE NEXT STATE *****
    std::vector<Creature*> allCreature;
    allCreature.reserve(allPred.size() + allPrey.size());
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
    m_target = allCreature[0];

    // Is there not a prey or predator nearby?
    if (!(allCreature[0]->m_distance < SHORT_DISTANCE*SHORT_DISTANCE)) {
        // Evade
        sm->changeState(new SMWander);
    }
    // ***** DETERMINE NEXT STATE FINISH *****
}

/****************************************************************************************************
****************************************************************************************************/
void SMEvade::update(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    determineNextState(creature, sm, allPred, allPrey, allFood);

    // Evade
    evadeTargets(creature, m_target);
    pushHome(creature);
}

/****************************************************************************************************
****************************************************************************************************/
void SMEvade::exit(Creature* creature) {
}

/****************************************************************************************************
****************************************************************************************************/
void SMEvade::evadeTargets(Creature* creature, Creature* target) {
    creature->m_maxSpeed = 5;

    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 direc = math::Vector3{ 0,0,0 };

    direc += (agentPos - ((Transform*)target->getComponent(comp::Transform))->getWorldTransform()[2]);


    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = agentPos + direc/2;
    direc.normalise();
    creature->m_direcForce = direc;
}