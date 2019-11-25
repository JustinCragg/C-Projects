#pragma once
#include "DTState.h"
#include "Creature.h"

// DTWander (Decision Tree Wander) is a state class which updates the creature
// When there are many no prey(blue) nearby or the pred(red) is not hungry
// The pred will move in a random forward direction
// A new target goal is determined when the pred reaches the target or after 5 game seconds 
class DTWander : public DTState {
public:
    // Constructor for the class
    DTWander(Creature* creature);

    // Used for initialising
    // Not used
    void init(Creature* creature);
    // Updates the input creature modifying it to act out the state
    // Modifies the creature's m_targetPos and m_direcForce to determine its intended movement
    void update(Creature* creature);
    // Used for deinitialising
    // Not used
    void exit(Creature* creature);

protected:
    // The function for selecting a new random goal to move towards
    void randGoal(Creature* creature);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Wander Algorithim Values
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // The radius of the circle
    float m_wandRadius = 20;
    // The distance away from the prey the circle is placed
    float m_wandDistance = 50;
    // The amount of random jitter added to the new target goal
    float m_wandJitter = 10;
};