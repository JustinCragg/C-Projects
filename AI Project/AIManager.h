#pragma once
#include "Creature.h"
#include <vector>

// A class used to handle the AI, movement and values of all the creatures
class AIManager {
public:
    // Updates each of the creatures, by calling their decision making component
    // This results in the m_targetPos and m_direcForce of each creature being modified
    void update(float deltaTime);
    // Moves each of the creatures by 'pushing' them modifying their m_velocity, moving them towards their target
    // Also rotates the creatures to look towards where they are moving
    void moveAll(float deltaTime);
    // Modifies each creatures values, and checks whether they become dead
    // Dead creatures are tagged as such and are no longer 'seen' by other creatures
    void checkDead(float deltaTime);

    // A vector containing all the preds(red)
    std::vector<Creature*> m_allPred;
    // A vector containing all the preys(blue)
    std::vector<Creature*> m_allPrey;
    // A vector containing all the foods(green)
    std::vector<Creature*> m_allFood;

protected:
    // Moves the creature towards their target by applying forces(m_direcForce) to their m_velocity
    void move(Creature* creature, float deltaTime);
    // Rotates the creature depending on their m_velocity, to look in the direction they are moving
    void look(Creature* creature);
};