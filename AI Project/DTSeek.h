#pragma once
#include "DTState.h"
#include "Creature.h"

// DTSeek (Decision Tree Seek) is a state class which updates the creature
// When the pred(red) is hungry and there is a prey(blue) nearby
// The pred will move towards the closest prey
class DTSeek : public DTState {
public:
    // Constructor for the class
    DTSeek(Creature* creature);

    // Used for initialising
    // Not used
    virtual void init(Creature* creature);
    // Updates the input creature modifying it to act out the state
    // Modifies the creature's m_targetPos and m_direcForce to determine its intended movement
    // target is the closest prey
    virtual void update(Creature* creature, Creature* target);
    // Used for deinitialising
    // Not used
    virtual void exit(Creature* creature);
protected:
    // The function for seeking towards the target creature
    void seekTarget(Creature* creature, Creature* target);
};