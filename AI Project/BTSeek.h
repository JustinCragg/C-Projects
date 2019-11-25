#pragma once
#include "BTState.h"

// BTSeek (Behaviour Tree Seek) is a state class which updates the creature
// When a prey(blue) is not near a pred(red) and the prey is hungry
// The prey will seek towards the closest food(green) and eat it
class BTSeek : public BTState {
public:
    // Constructor for the class
    BTSeek(Creature* creature);

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
    // The function for seeking towards the target food
    void seekTarget(Creature* creature);
};