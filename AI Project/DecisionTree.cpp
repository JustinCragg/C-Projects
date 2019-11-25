#include "DecisionTree.h"
#include "DTQuestion.h"
#include "DTAnswer.h"

#include "DTEvade.h"
#include "DTSeek.h"
#include "DTWander.h"

#include "Creature.h"
#include "Predator.h"

#include <Functions.h>
#include <algorithm>

/****************************************************************************************************
****************************************************************************************************/
DecisionTree::DecisionTree(SceneObject* parent) : Component (parent) {
    m_componentType = comp::DecisionTree;

    Creature* creature = (Creature*)parent;

    m_startNode = new DTQuestion{ &(creature->m_scared) };
    ((DTQuestion*)m_startNode)->newNode(true, new DTAnswer{ new DTEvade{creature} });
    ((DTQuestion*)m_startNode)->newNode(false, new DTQuestion{ &(creature->m_hungry) });
    ((DTQuestion*)((DTQuestion*)m_startNode)->m_falseBranch)->newNode(true, new DTQuestion{ &(creature->m_hunting) });
    ((DTQuestion*)((DTQuestion*)m_startNode)->m_falseBranch)->newNode(false, new DTAnswer{ new DTWander{creature} });
    ((DTQuestion*)((DTQuestion*)((DTQuestion*)m_startNode)->m_falseBranch)->m_trueBranch)->newNode(true, new DTAnswer{ new DTSeek{creature} });
    ((DTQuestion*)((DTQuestion*)((DTQuestion*)m_startNode)->m_falseBranch)->m_trueBranch)->newNode(false, new DTAnswer{ new DTWander{creature} });
}

/****************************************************************************************************
****************************************************************************************************/
void DecisionTree::determineValues(std::vector<Creature*> allPreys) {
    Predator* creature = (Predator*)m_parent;

    creature->m_scared = false;
    // Only resets hungry if the creature is full or there is nothing nearby to eat
    if (!(creature->m_hunting == true && creature->m_hunger <= 90)) {
        creature->m_hungry = false;
    }
    creature->m_hunting = false;
    creature->m_preys.clear();
    std::vector<Creature*> temps;

    for (Creature* prey : allPreys) {
        if (prey->dead == false) {
            temps.push_back(prey);
            // Calculate distance between each pred and the prey
            prey->m_distance = math::distanceBetweenSq(((Transform*)prey->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)m_parent->getComponent(comp::Transform))->getWorldTransform()[2]);
        }
    }

    // Sort prey by distance to predator
    std::sort(temps.begin(), temps.end(), lessThan());

    // Determine if there are lots of nearby prey
    int closePrey = 0;
    for (int i = 0; i < temps.size(); i++) {
        if (temps[i]->m_distance < SHORT_DISTANCE*SHORT_DISTANCE) {
            closePrey++;
            creature->m_preys.push_back(temps[i]);
            if (closePrey >= 4) {
                break;
            }
        }
    }

    // Determine if the predator is hungry
    if (creature->m_hunger < HUNGRY_THRESHOLD) {
        creature->m_hungry = true;
    }

    // Determine if there are nearby prey
    if (closePrey > 0) {
        creature->m_hunting = true;
    }

    // Determine if there are nearby prey
    if (closePrey >= 5) {
        creature->m_scared = true;
    }
}

/****************************************************************************************************
****************************************************************************************************/
void DecisionTree::update(std::vector<Creature*> allPreys) {
    determineValues(allPreys);
    m_startNode->makeDecision((Creature*)m_parent);
}