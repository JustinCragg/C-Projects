#include "AIManager.h"
#include "StateMachine.h"
#include "DecisionTree.h"
#include "BehaviourTree.h"
#include "Predator.h"
#include "Game.h"
#include <Functions.h>
#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
void AIManager::look(Creature* creature) {
    // Look at direction
    float angle = creature->m_velocity.y / creature->m_velocity.x;
    if (creature->m_velocity.x == 0 && creature->m_velocity.y == 0) {
        angle = 0;
    }
    angle = atan(angle);
    ((Transform*)creature->getComponent(comp::Transform))->setRotation(angle);
}

/****************************************************************************************************
****************************************************************************************************/
void AIManager::move(Creature* creature, float deltaTime) {
    float accel = 5;

    creature->m_velocity += creature->m_direcForce;
    // Accelerate
    if (creature->m_speed < creature->m_maxSpeed) {
        creature->m_speed += deltaTime * accel;
    }
    // Deccelerate
    else if (creature->m_speed > creature->m_maxSpeed) {
        creature->m_speed -= deltaTime * accel;
    }
    math::clamp(creature->m_speed, 0, 25);


    math::Vector3 zero;
    if (!(creature->m_velocity == zero)) {
        creature->m_velocity.normalise();
    }
    creature->m_velocity *= creature->m_speed;

    look(creature);

    float distanceToTarget = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], creature->m_targetPos);
    float moveDistance = creature->m_velocity.magnitudeSq() * deltaTime;

    // If the creature will not reach the target
    if (distanceToTarget > moveDistance) {
        // Move Normally
        ((Transform*)creature->getComponent(comp::Transform))->movePosition(creature->m_velocity * deltaTime);
    }
    // If the creature will reach the target
    else {
        // Move directly to target
        ((Transform*)creature->getComponent(comp::Transform))->setPosition(creature->m_targetPos);
    }
}

/****************************************************************************************************
****************************************************************************************************/
void AIManager::update(float deltaTime) {
    for (Creature* pred : m_allPred) {
        if (pred->dead == false) {
            ((DecisionTree*)pred->getComponent(comp::DecisionTree))->update(m_allPrey);
        }
    }
    for (Creature* prey : m_allPrey) {
        if (prey->dead == false) {
            ((BehaviourTree*)prey->getComponent(comp::BehaviourTree))->update(m_allPred, m_allPrey, m_allFood);
        }
    }
    for (Creature* food : m_allFood) {
        if (food->dead == false) {
            ((StateMachine*)food->getComponent(comp::StateMachine))->update(m_allPred, m_allPrey, m_allFood);
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
void AIManager::moveAll(float deltaTime) {
    // Predators
    for (Creature* pred : m_allPred) {
        if (pred->dead == false) {            
            move(pred, deltaTime);
        }
    }
    // Preys
    for (Creature* prey : m_allPrey) {
        if (prey->dead == false) {            
            move(prey, deltaTime);
        }
    }
    // Foods
    for (Creature* food: m_allFood) {
        if (food->dead == false) {
            move(food, deltaTime);
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
void AIManager::checkDead(float deltaTime) {
    for (Creature* pred : m_allPred) {
        if (pred->dead == false) {
            // Decrease values
            pred->m_health += 0.5f * deltaTime;
            pred->m_health = math::clamp(pred->m_health, 0, 100);
            pred->m_hunger -= 0.25f*deltaTime;

            // Check if starving
            if (pred->m_hunger < 0) {
                pred->m_health += (pred->m_hunger*0.25f)*deltaTime;
            }
            // Check if dead
            if (pred->m_health <= 0) {
                pred->dead = true;
                pred->m_distance = INFINITY;
            }
        }
        else {
            // Respawns the pred if it has been dead for longer than 30 game seconds
            if (Game::getGameTime() - pred->m_deathTime > 30) {
                pred->m_health = (float)(pred->m_randGen.rand() % 50);
                pred->m_health += 50;
                pred->m_hunger = (float)(pred->m_randGen.rand() % 50);
                pred->m_hunger += 50;

                int x = pred->m_randGen.rand() % 1280;
                x -= 640;
                int y = pred->m_randGen.rand() % 720;
                y -= 360;

                ((Transform*)pred->getComponent(comp::Transform))->setPosition((float)x, (float)y);

                pred->dead = false;
            }
        }
    }
    for (Creature* prey : m_allPrey) {
        if (prey->dead == false) {
            // Decrease values
            prey->m_health += 0.5f * deltaTime;
            if (prey->m_nearHome == true) {
                prey->m_health += 0.5f * deltaTime;
            }
            prey->m_health = math::clamp(prey->m_health, 0, 100);
            prey->m_hunger -= 1.5f*deltaTime;

            // Check if starving
            if (prey->m_hunger < 0) {
                prey->m_health += (prey->m_hunger*0.75f)*deltaTime;
            }
            // Check if dead
            if (prey->m_health <= 0) {
                prey->m_deathTime = Game::getGameTime();
                prey->dead = true;
                prey->m_distance = INFINITY;
            }
        }
        else {
            // Respawns the pred if it has been dead for longer than 20 game seconds
            if (Game::getGameTime() - prey->m_deathTime > 20) {
                prey->m_health = (float)(prey->m_randGen.rand() % 50);
                prey->m_health += 50;
                prey->m_hunger = (float)(prey->m_randGen.rand() % 50);
                prey->m_hunger += 50;

                int x = prey->m_randGen.rand() % 1280;
                x -= 640;
                int y = prey->m_randGen.rand() % 720;
                y -= 360;

                ((Transform*)prey->getComponent(comp::Transform))->setPosition((float)x, (float)y);

                prey->dead = false;
            }
        }
    }
    for (Creature* food : m_allFood) {
        if (food->dead == false) {
            // Check if dead
            if (food->m_health <= 0) {
                food->m_deathTime = Game::getGameTime();
                food->dead = true;
                food->m_distance = INFINITY;
            }
        }
        else {
            // Respawns the pred if it has been dead for longer than 10 game seconds
            if (Game::getGameTime() - food->m_deathTime > 10) {
                food->m_health = 10;

                int x = food->m_randGen.rand() % 1280;
                x -= 640;
                int y = food->m_randGen.rand() % 720;
                y -= 360;

                ((Transform*)food->getComponent(comp::Transform))->setPosition((float)x, (float)y);

                food->dead = false;
            }
        }
    }
}