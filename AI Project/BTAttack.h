#pragma once
#include "BTState.h"

// BTAttack (Behaviour Tree Attack) is a state class which updates the creature
// When a prey(blue) is near a pred(red), the prey's health is not low, the prey is near its home and has other prey nearby
// The prey will seek towards the closest pred to its home and attack it
class BTAttack : public BTState {
public:
    // Constructor for the class
    BTAttack(Creature* creature);

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
    // The function for seeking towards the enemy
    void seekEnemy(Creature* creature);
};