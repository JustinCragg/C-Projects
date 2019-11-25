#include "BehaviourTree.h"

#include "BTSelector.h"
#include "BTSequence.h"
#include "BTAction.h"
#include "BTCondition.h"

#include "BTFlee.h"
#include "BTAttack.h"
#include "BTEvade.h"
#include "BTSeek.h"
#include "BTPatrol.h"
#include "BTWander.h"

#include "Prey.h"
#include "Game.h"
#include <Functions.h>
#include <algorithm>

/****************************************************************************************************
****************************************************************************************************/
BehaviourTree::BehaviourTree(SceneObject* parent) : Component(parent) {
    m_componentType = comp::BehaviourTree;

    Creature* creature = (Creature*)m_parent;

    //    _________________0_______________
    // ____1____          _2_          ___3___
    // ?   ____4____      ? !         _5_    !
    //    _6_   ___7___               ? !
    //    ? ! __8__   !
    //        ? ? !

    m_start = new BTSelector; // 0

    ((BTSelector*)m_start)->addChild(new BTSequence); // 0-1
    ((BTSequence*)((BTSelector*)m_start)->m_children[0])->addChild(new BTCondition{&(creature->m_scared)}); // 0-1-?
    ((BTSequence*)((BTSelector*)m_start)->m_children[0])->addChild(new BTSelector);  //                        0-1-4
    ((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->addChild(new BTSequence); // 0-1-4-6
    ((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->addChild(new BTSelector); // 0-1-4-7
    ((BTSequence*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[0])->addChild(new BTCondition{ &(creature->m_hurting) }); // 0-1-4-6-?
    ((BTSequence*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[0])->addChild(new BTAction{ new BTFlee{creature} }); //      0-1-4-6-!
    ((BTSelector*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[1])->addChild(new BTSequence); //                          0-1-4-7-8
    ((BTSelector*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[1])->addChild(new BTAction{ new BTEvade{creature} }); //   0-1-4-7-!
    ((BTSequence*)((BTSelector*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[1])->m_children[0])->addChild(new BTCondition{&(creature->m_nearHome)}); // 0-1-4-7-8-?
    ((BTSequence*)((BTSelector*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[1])->m_children[0])->addChild(new BTCondition{ &(creature->m_protected) }); // 0-1-4-7-8-?
    ((BTSequence*)((BTSelector*)((BTSelector*)((BTSequence*)((BTSelector*)m_start)->m_children[0])->m_children[1])->m_children[1])->m_children[0])->addChild(new BTAction{ new BTAttack{creature} }); // 0-1-4-7-8-!

    ((BTSelector*)m_start)->addChild(new BTSequence); // 0-2
    ((BTSequence*)((BTSelector*)m_start)->m_children[1])->addChild(new BTCondition{ &(creature->m_hungry) }); // 0-2-?
    ((BTSequence*)((BTSelector*)m_start)->m_children[1])->addChild(new BTAction{ new BTSeek{creature} }); // 0-2-!

    ((BTSelector*)m_start)->addChild(new BTSelector); // 0-3
    ((BTSelector*)((BTSelector*)m_start)->m_children[2])->addChild(new BTSequence); // 0-3-5
    ((BTSelector*)((BTSelector*)m_start)->m_children[2])->addChild(new BTAction{ new BTWander{creature} }); // 0-3-!
    ((BTSequence*)((BTSelector*)((BTSelector*)m_start)->m_children[2])->m_children[0])->addChild(new BTCondition{ &(creature->m_nearHome) }); // 0-3-5-?
    ((BTSequence*)((BTSelector*)((BTSelector*)m_start)->m_children[2])->m_children[0])->addChild(new BTAction{ new BTPatrol{creature} }); // 0-3-5-!
}

/****************************************************************************************************
****************************************************************************************************/
void BehaviourTree::determineValues(std::vector<Creature*> allPreds, std::vector<Creature*> allPreys, std::vector<Creature*> allFoods) {
    Creature* creature = (Creature*)m_parent;
    Prey* preyCreature = (Prey*)m_parent;

    // Clear the vectors
    preyCreature->m_preds.clear();
    preyCreature->m_preys.clear();
    preyCreature->m_foods.clear();

    // Reset booleans
    // Only resets hungry if the creature is full or there is nothing nearby to eat
    if (creature->m_hunger >= 90 || creature->m_scared == true || creature->m_hurting == true) {
        creature->m_hungry = false;
    }
    creature->m_hurting = false;
    creature->m_scared = false;
    creature->m_nearHome = false;
    creature->m_protected = false;

    if (Game::getGameTime() - preyCreature->m_homeTimer >= 30 || preyCreature->m_homeTimer == 0) {
        preyCreature->m_homeTimer = Game::getGameTime();
        preyCreature->addHomePoint();
    }

    // Near the enemy
    int closePred = 0;
    for (Creature* pred : allPreds) {
        if (pred->dead == false) {
            preyCreature->m_preds.push_back(pred);
            pred->m_distance = math::distanceBetweenSq(((Transform*)pred->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)m_parent->getComponent(comp::Transform))->getWorldTransform()[2]);
            if (pred->m_distance <= SHORT_DISTANCE * SHORT_DISTANCE) {
                closePred++;
            }
        }
    }
    std::sort(((Prey*)creature)->m_preds.begin(), ((Prey*)creature)->m_preds.end(), lessThan());

    // Near preys
    int closePrey = 0;
    for (Creature* prey : allPreys) {
        if (prey->dead == false) {
            preyCreature->m_preys.push_back(prey);
            prey->m_distance = math::distanceBetweenSq(((Transform*)prey->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)m_parent->getComponent(comp::Transform))->getWorldTransform()[2]);
            if (prey->m_distance <= SHORT_DISTANCE * SHORT_DISTANCE) {
                closePrey++;
            }
        }
    }
    std::sort(((Prey*)creature)->m_preys.begin(), ((Prey*)creature)->m_preys.end(), lessThan());
    if (Game::getGameTime() - preyCreature->m_homeTimer >= 15 || preyCreature->m_homeTimer == 0) {
        preyCreature->m_homeTimer = Game::getGameTime();
        preyCreature->addHomePoint(preyCreature->m_preys[0]->m_homePos);
    }

    // Foods
    for (Creature* food : allFoods) {
        if (food->dead == false) {
            ((Prey*)creature)->m_foods.push_back(food);
            food->m_distance = math::distanceBetweenSq(((Transform*)food->getComponent(comp::Transform))->getWorldTransform()[2], ((Transform*)m_parent->getComponent(comp::Transform))->getWorldTransform()[2]);
            if (food->m_distance <= SHORT_DISTANCE * SHORT_DISTANCE) {
                closePrey++;
            }
        }
    }
    std::sort(((Prey*)creature)->m_foods.begin(), ((Prey*)creature)->m_foods.end(), lessThan());

    // Determine booleans
    if (closePred > 0) {
        creature->m_scared = true;
    }
    if (creature->m_health <= HURT_THRESHOLD) {
        creature->m_hurting = true;
    }
    if (creature->m_hunger <= HUNGRY_THRESHOLD && ((Prey*)creature)->m_foods.empty() == false) {
        creature->m_hungry = true;
    }
    if (math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_homePos) <= SHORT_DISTANCE) {
        creature->m_nearHome = true;
    }
    if (closePrey >= 3) {
        creature->m_protected = true;
    }
}

/****************************************************************************************************
****************************************************************************************************/
void BehaviourTree::update(std::vector<Creature*> allPreds, std::vector<Creature*> allPreys, std::vector<Creature*> allFoods) {
    determineValues(allPreds, allPreys, allFoods);
    m_start->execute((Creature*)m_parent);
}