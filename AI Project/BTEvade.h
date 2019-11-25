#pragma once
#include "BTState.h"

// BTEvade (Behaviour Tree Evade) is a state class which updates the creature
// When a prey(blue) is near a pred(red) and the prey's health is not low
// The prey will move directly away from all preds that can be seen
class BTEvade : public BTState {
public:
    // Constructor for the class
    BTEvade(Creature* creature);

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
    // The funtion for evadeing the enemy targets
    void evadeTargets(Creature* creature);
};