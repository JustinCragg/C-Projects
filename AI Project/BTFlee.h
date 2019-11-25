#pragma once
#include "BTState.h"

// BTFlee (Behaviour Tree Flee) is a state class which updates the creature
// When a prey(blue) is near a pred(red) and the prey's health is low
// The prey will use pathfinding to determine a route to its home which minimises proximity to any preds
// The route is updated every 5 game seconds
// While also moving slightly away from nearby preds constantly
class BTFlee : public BTState {
public:
    // Constructor for the class
    BTFlee(Creature* creature);

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
    // The function for seeking towards the next node in the route
    void seekNode(Creature* creature);
    // A constant force pushing the prey away from the closest pred it can see
    void evadeEnemy(Creature* creature);
};