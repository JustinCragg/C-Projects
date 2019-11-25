#pragma once
#include "DTState.h"
#include "Creature.h"

// DTEvade (Decision Tree Evade) is a state class which updates the creature
// When there are many prey(blue) nearby to the pred(red)
// The pred will move directly away from prey it can see
class DTEvade : public DTState {
public:
    // Constructor for the class
    DTEvade(Creature* creature);

    // Used for initialising
    // Not used
    virtual void init(Creature* creature);
    // Updates the input creature modifying it to act out the state
    // Modifies the creature's m_targetPos and m_direcForce to determine its intended movement
    // Uses targets to determine which creatures to evade
    virtual void update(Creature* creature, std::vector<Creature*> targets);
    // Used for deinitialising
    // Not used
    virtual void exit(Creature* creature);

protected:
    // The function for evading the target enemies
    void evadeTargets(Creature* creature, std::vector<Creature*> targets);
};