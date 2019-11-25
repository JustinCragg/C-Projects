#pragma once
#include "Creature.h"
#include "Transform.h"
#include <Vector3.h>
#include <vector>
#include <deque>

// Contatins the components; sprite, transform, navAgent, behaviourTree
class Prey : public Creature {
public:
    // The class constructor
    // exePath is the direct path to the program's exe, transfromParent is the parent of the Transform component
    // navMap is a reference to the navMap for the creature to use
    Prey(std::string exePath, Transform* transformParent, NavMap* navMap);
    // Initialises the class
    void init(std::string exePath, Transform* transformParent, NavMap* navMap);
    // Destructor
    ~Prey();

    // Adds the creature's currentPosition to the vector of homePoints
    void addHomePoint();
    // Adds a specific position to the vector of homePoints
    void addHomePoint(math::Vector3 pos);

    // A vector of the preys
    std::vector<Creature*> m_preys;
    // A vector of the preds
    std::vector<Creature*> m_preds;
    // A vector of the foods
    std::vector<Creature*> m_foods;

    // A timer which stores when the last homePoint was stored
    float m_homeTimer = 0;
    // A timer which stores when the last time something was eaten
    float m_eatTimer = 0;
    // A timer which stores when the path was last updated
    float m_pathingTimer = 0;

    // A list of points which the creature's homePos is determined from
    std::deque<math::Vector3> m_homePoints;
};