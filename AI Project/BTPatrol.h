#pragma once
#include "BTState.h"

// BTPatrol (Behaviour Tree Patrol) is a state class which updates the creature
// When a prey(blue) is not near a pred(red), the prey's health is not low and the prey is near its home
// The prey will move in a circle around its home
class BTPatrol: public BTState {
public:
    // Constructor for the class
    BTPatrol(Creature* creature);

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
    // The function for seeking towards the next point on the circles circumphrence
    void seekTarget(Creature* creature);

    // The angle on the circle which is currently used at the target
    float m_angle = 0;
};