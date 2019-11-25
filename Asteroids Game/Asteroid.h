#pragma once
#include "Sprite.h"
#include "Ship.h"
#include <Vector3.h>
#include <coreXorShift.h>

struct Asteroid : public Sprite {
public:
    using Sprite::Sprite;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Set Functions
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Sets the direction of the asteroid's velocity
    void setDirec(math::Vector3 direc);
    // Sets the direction of the asteroid's velocity
    void setDirec(float x, float y);
    // Sets spin of the asteroid (-1,1)
    void setSpin(float speed);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Random Set Functions
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calls each of the random functions - randPos, randDir, randSpin
    // Also flags the asteroid to be displayed
    void randSet(Ship* player);
    // Randomly sets the asteroid's position, to be within a certain distance of the player, but not on the screen
    void randPos(Ship* player);
    // Randomly sets the asteroid's velocity
    void randDir();
    // Randomly sets the asteroid's spin (-1,1)
    void randSpin();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Movement
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Moves the asteroid according to its velocity (direction)
    void autoMove(float deltaTime, Ship* player);

    // The direction the asteroid moves in
    // Speed is magnitude
    math::Vector3 m_direc;
    // The speed the asteroid spins at
    float m_spin = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Collider
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Sets the colliders for the asteroid
    void setupCollider();
private:
    // Creates random numbers
    core::XorShift32 m_rand;
};