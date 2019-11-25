#pragma once
#include "SMState.h"
#include "StateMachine.h"
#include "Creature.h"
#include <Vector3.h>
#include <string>

// SMEvade (State Machine Evade) is a state class which updates the creature and determines which state to be executed next frame
// When there are preds(red) or prey(blue) nearby
// Move directly away from the closest pred/prey
class SMEvade : public SMState {
public:
    // Constructor for the class
    SMEvade();

    // Used for initialising
    // Not used
    virtual void init(Creature* creature);
    // Updates the input creature modifying it to act out the state
    // Modifies the creature's m_targetPos and m_direcForce to determine its intended movement
    // Uses the vectors of creatures to determine what to evade and which state to execute next frame
    virtual void update(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood);
    // Used for deinitialising
    // Not used
    virtual void exit(Creature* creature);

protected:
    // Is called by update to determine what teh state will be called next frame
    // Decision making is determined in this funtion and so vectors of the creatures are needed
    void determineNextState(Creature* creature, StateMachine* sm, std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood);

    // The function for evading the target
    void evadeTargets(Creature* creature, Creature* target);

    // The target creature
    Creature* m_target;
};