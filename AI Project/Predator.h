#pragma once
#include "Creature.h"
#include "Transform.h"

// Contains the components; sprite, transform, decisionTree
class Predator : public Creature {
public:
    // The class constructor
    // exePath is the direct path to the program's exe, transfromParent is the parent of the Transform component
    Predator(std::string exePath, Transform* transformParent);
    // Initialises the class
    void init(std::string exePath, Transform* transformParent);
    // Destructor
    ~Predator();

    // A vector listing all the preys
    std::vector<Creature*> m_preys;
};