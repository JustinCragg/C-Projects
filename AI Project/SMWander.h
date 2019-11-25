#pragma once
#include "SMState.h"
#include "StateMachine.h"
#include "Creature.h"
#include <Vector3.h>
#include <string>

// SMWander (State Machine Wander) is a state class which updates the creature and determines which state to be executed next frame
// When there are no preds(red) or prey(blue) nearby and there are food(green) close
// Move in a random direction
// A new target goal is determined when the pred reaches the target or after 5 game seconds 
class SMWander : public SMState {
public:
    // Constructor for the class
    SMWander();

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

    // The function for selecting a new random goal to move towards
	void randGoal(Creature* creature);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Wander Algorithim Values
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // The radius of the circle
    float m_wandRadius = 5;
    // The distance away from the food the circle is palced
    float m_wandDistance = 2.5f;
    // The amount of random jitter added to the new target goal
    float m_wandJitter = 2.5f;

};