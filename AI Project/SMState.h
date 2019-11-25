#pragma once
#include <Vector3.h>
#include "Creature.h"

// Checks if moving in the direction will result in moving of the screen and if so modifies the direction by flipping the offending axis
math::Vector3 screenBounce(math::Vector3 origin, math::Vector3 direc);

// Enum listing the possible stae which can be executed
namespace state {
    enum state { Evade, None, Seek, Wander };
}

// Overarching parent class
// SMState (State Machine State) is the base state which other states inherit from
// Regardless of the state each food(green) has a slight push toward their home
class SMState {
public:
    // The generic funciton which all children must overide
    // Used for initialising a state
    virtual void init(Creature* creature) = 0;
    // The generic funciton which all children must overide
    // Used for updating the input creature accoridng to the pertaining state
    // Decision making is determined in this function and so vectors of the creatures are needed
    virtual void update(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) = 0;
    // The generic funciton which all children must overide
    // Used for deinitialising a state
    virtual void exit(Creature* creature) = 0;

    // The type of state created
    state::state m_stateType = state::None;


protected:
    // Is called by update to determine what the state will be called next frame
    // Decision making is determined in this function and so vectors of the creatures are needed
    virtual void determineNextState(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) = 0;

    // Creates a slight push toward the food's home
    // Modifies the creature's m_targetPos and m_direcForce to determine intended movement
    void pushHome(Creature* creature);

    // Used to store the game time when certain actions are carried out in the states
    float m_time = 0;
};