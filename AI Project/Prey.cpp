#include "Prey.h"
#include "BehaviourTree.h"

/****************************************************************************************************
****************************************************************************************************/
Prey::Prey(std::string exePath, Transform* transformParent, NavMap* navMap) {
    init(exePath, transformParent, navMap);
}

/****************************************************************************************************
****************************************************************************************************/
void Prey::init(std::string exePath, Transform* transformParent, NavMap* navMap) {
    m_filePath = "/textures/prey.png";
    Creature::init(exePath, transformParent, navMap);
    addComponent(new BehaviourTree{ this });

    m_homeTimer = 0;
}

/****************************************************************************************************
****************************************************************************************************/
Prey::~Prey() {

}

/****************************************************************************************************
****************************************************************************************************/
void Prey::addHomePoint() {
    m_homePoints.push_back(((Transform*)getComponent(comp::Transform))->getWorldTransform()[2]);
    // Restrict the size of the dequeue to 5, removing the oldest position
    if (m_homePoints.size() > 5) {
        m_homePoints.pop_front();
    }

    // Recalculate the homePos by averaging the positions in homePos
    math::Vector3 tempPos;
    for (math::Vector3 pos : m_homePoints) {
        tempPos += pos;
    }
    tempPos /= (float)m_homePoints.size();
    m_homePos = tempPos;
}

/****************************************************************************************************
****************************************************************************************************/
void Prey::addHomePoint(math::Vector3 pos) {
    // Restrict the size of the dequeue to 5, removing the oldest position
    m_homePoints.push_back(pos);
    if (m_homePoints.size() > 5) {
        m_homePoints.pop_front();
    }

    // Recalculate the homePos by averaging the positions in homePos
    math::Vector3 tempPos;
    for (math::Vector3 pos : m_homePoints) {
        tempPos += pos;
    }
    tempPos /= (float)m_homePoints.size();
    m_homePos = tempPos;
}