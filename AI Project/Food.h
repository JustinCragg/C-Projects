#pragma once
#include "Creature.h"
#include "Transform.h"

// Contains the components; Sprite, Transform, StateMachine
class Food : public Creature {
public:
    // The class constructor
    // exePath is the direct path to the program's exe, transfromParent is the parent of the Transform component
    Food(std::string exePath, Transform* transformParent);
    // Initialises the class
    void init(std::string exePath, Transform* transformParent);
    // Destructor
    ~Food();

    // Calls the update function for the StateMachine component which handles the AI decision making
    void update(std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood);
};