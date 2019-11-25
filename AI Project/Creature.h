#pragma once
#include "SceneObject.h"
#include "Transform.h"
#include <string>
#include <coreXorShift.h>
#include <coreTimer.h>

// The distance in pixels considered to be the creature's sight range
const float SHORT_DISTANCE = 150;
// The threshold in which being below considers the creature to be hurt
const int HURT_THRESHOLD = 40;
// The threshold in which being below considers the creature to be hungry
const float HUNGRY_THRESHOLD = 60;

class StateMachine;
class NavMap;

// An overarching class for creatures
// Stores the common values for every creature
class Creature : public SceneObject {
public:
    // The class constructor
    Creature();
    // Initialises the class
    // exePath is the direct path to the program's exe, transfromParent is the parent of the Transform component
    // navMap is a reference to the navMap for the creature to use
    void init(std::string exePath, Transform* transformParent, NavMap* navMap = nullptr);
    // The class destructor
    ~Creature();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Values
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Whether the creature has died
    bool dead = false;
    // The creatures health
    float m_health = 100;
    // The creatures hugner
    float m_hunger = 100;

    // The position in which the creature wants to move
    math::Vector3 m_targetPos{ 0,0,1 };
    // The position of the creature's home
    math::Vector3 m_homePos{ INFINITY, INFINITY, INFINITY };

    // The current speed the creature is moving at
    float m_speed = 15;
    // The maximum speed the creature can move at
    float m_maxSpeed = 25;
    // The creature's current velocity
    math::Vector3 m_velocity;
    // The 'push' force which is added to the velocity
    math::Vector3 m_direcForce;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 'Feelings'
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Whether the creature is threatened by other creatures
    bool m_scared = false;
    // Whether the creature has low health
    bool m_hurting = false;
    // Whether the creature is in a safe position and near allies
    bool m_protected = false;
    // Whether the creature is hungry
    bool m_hungry = false;
    // Whether the creature is currently hunting for food
    bool m_hunting = false;
    // Whether the creature is currently near its home
    bool m_nearHome = false;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Misc.
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // A random number generator
    core::XorShift32 m_randGen;

    // A real-time timer
    core::Timer m_timer;
    // A float which stores the time of the creatures death in game time
    float m_deathTime = 0;

    // Used to store the distance between this creature and another
    float m_distance;

protected:
    // The path from the exe folder to the creature's texture
    std::string m_filePath;
};

// A struct which can be used to sort creatures by their distances to a point
struct lessThan {
    bool operator()(Creature* a, Creature* b) {
        return a->m_distance < b->m_distance;
    }
};