#include "SMState.h"
#include "Food.h"
#include <Functions.h>

/****************************************************************************************************
****************************************************************************************************/
math::Vector3 screenBounce(math::Vector3 origin, math::Vector3 direc) {
    math::Vector3 worldPos = origin + direc;
    // Horizontally off the screen
    if (worldPos.x > 640 || worldPos.x < -640) {
        direc.x *= -1;
    }
    // Vertically off the screen
    if (worldPos.y > 360 || worldPos.y < -360) {
        direc.y *= -1;
    }
    return direc;
}

/****************************************************************************************************
****************************************************************************************************/
void SMState::pushHome(Creature* creature) {
    math::Vector3 agentPos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    math::Vector3 targetPos = creature->m_homePos;

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);

    // Only pushes if the creature is not already at their home
    if (distanceToTarget > 75 * 75) {
        math::Vector3 direc = targetPos - agentPos;
        // The push is diminished so it makes minimal impact on the overall movement of the creature
        direc *= 0.01f;

        direc = screenBounce(agentPos, direc);

        creature->m_targetPos += targetPos;
        direc.normalise();
        creature->m_direcForce += direc;
    }
}