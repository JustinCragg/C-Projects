#include "NavAgent.h"
#include "Transform.h"

/****************************************************************************************************
****************************************************************************************************/
NavAgent::NavAgent(SceneObject* parent, NavMap* navMap) : Component(parent) {
    m_componentType = comp::NavAgent;

    init(navMap);
}

/****************************************************************************************************
****************************************************************************************************/
void NavAgent::init(NavMap * navMap) {
    m_navMap = navMap;
}

/****************************************************************************************************
****************************************************************************************************/
NavAgent::~NavAgent() {}

/****************************************************************************************************
****************************************************************************************************/
void NavAgent::calculatePath(math::Vector3 target) {
    m_target = target;
    m_navMap->resetNodes();

    Transform* transform = (Transform*)m_parent->getComponent(comp::Transform);
    Node* startNode = m_navMap->getClosestNode(transform->getPosition());
    Node* endNode = m_navMap->getClosestNode(m_target);

    m_nodePath = m_navMap->aStarSearch(startNode, endNode);
    m_gotPath = true;
}

/****************************************************************************************************
****************************************************************************************************/
void NavAgent::calculatePath(math::Vector3 target, std::vector<Creature*> enemies) {
    m_target = target;
    m_navMap->resetNodes();

    Transform* transform = (Transform*)m_parent->getComponent(comp::Transform);
    Node* startNode = m_navMap->getClosestNode(transform->getPosition());
    Node* endNode = m_navMap->getClosestNode(m_target);

    m_nodePath = m_navMap->aStarSearch(startNode, endNode, enemies);
    m_gotPath = true;
}

/****************************************************************************************************
****************************************************************************************************/
void NavAgent::navMove(float step) {
    if (m_gotPath == true) {
        Transform* transform = (Transform*)m_parent->getComponent(comp::Transform);
        if (m_nodePath.empty() == true) {
            // If there are no more nodes left, move straight for the target
            if (transform->moveTowards(m_target, step) == true) {
                m_gotPath = false;
            }
        }
        else {
            // If the next node has been reached, remove the once just reached
            if (transform->moveTowards(m_nodePath.back()->pos, step) == true) {
                m_nodePath.pop_back();
            }
        }
    }
}