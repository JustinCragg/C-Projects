#pragma once
#include "BTState.h"

// BTWander (Behaviour Tree Wander) is a state class which updates the creature
// When a prey(blue) is not near a pred(red) and the prey is not hungry
// The prey will move in a random forward direction
// A new target goal is determined when the prey reaches the target or after 5 game seconds 
class BTWander : public BTState {
public:
    // Constructor for the class
    BTWander(Creature* creature);

    // Used for initialising the state
    // Not used
    virtual void init(Creature* creature);
    // Updates the input creature modifying it to act out the state
    // Modifies the creature's m_targetPos and m_direcForce to determine its intended movement
    virtual result::result update(Creature* creature);
    // Used for deinitialising the state
    // Not used
    virtual void exit(Creature* creature);

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