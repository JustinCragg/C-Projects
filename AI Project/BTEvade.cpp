#include "BTEvade.h"
#include "Prey.h"
#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
BTEvade::BTEvade(Creature* creature) {
    m_stateType = state::Evade;
}

/****************************************************************************************************
****************************************************************************************************/
void BTEvade::init(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
result::result BTEvade::update(Creature* creature) {
    evadeTargets(creature);

    return result::Success;
}

/****************************************************************************************************
****************************************************************************************************/
void BTEvade::exit(Creature* creature) {

}

/****************************************************************************************************
****************************************************************************************************/
void BTEvade::evadeTargets(Creature* creature) {
    Prey* prey = (Prey*)creature;
    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 direc = math::Vector3{ 0,0,0 };
    float avDist = 0;
    int num = 0;

    for (Creature* target : prey->m_preds) {
        if (target->m_distance < SHORT_DISTANCE*SHORT_DISTANCE) {
            num++;
            avDist += target->m_distance;
            // Compiles all the directions to the nearby preds
            direc += (agentPos - ((Transform*)target->getComponent(comp::Transform))->getWorldTransform()[2]);
        }
    }
    // Determines the average distance to each of the nearby preds
    avDist /= num;
    avDist = sqrt(avDist);

    direc.normalise();
    direc *= avDist;

    direc = screenBounce(agentPos, direc);

    creature->m_targetPos = agentPos + direc;
    direc.normalise();
    creature->m_direcForce = direc;
}