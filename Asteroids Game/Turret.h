#pragma once
#include "Sprite.h"

const int NUM_BALLS = 5;

class Turret : public Sprite {
public:
    using Sprite::Sprite;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Orbiting balls
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Adds orbiting balls
    void addTurret();
    // Moves the balls to orbit the object
    void turretMove(float deltaTime);
    // The pool of balls
    std::vector<Turret*> m_turretPool;

    // Draws the sprites in the ball pool then the ball
    virtual void draw(aie::Renderer2D* renderer);

    // The angle in the orbit the ball is currently at
    float m_angle = 0;
};