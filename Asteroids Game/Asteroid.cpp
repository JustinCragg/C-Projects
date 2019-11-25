#include "Asteroid.h"
#include "Physics.h"
#include "Ship.h"

#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::autoMove(float deltaTime, Ship* player) {
    if (getDisplayed() == true) {
        // Move in direction
        movePosition(m_direc*deltaTime);

        if (distanceBetweenSqr(*this, *player) > 2000*2000) {
            // Too far away, destroy
            setDisplayed(false);
        }

        rotateRotation(m_spin*PI*deltaTime);
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::setDirec(math::Vector3 direc) {
    m_direc = direc;
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::setDirec(float x, float y) {
    m_direc.x = x;
    m_direc.y = y;
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::setSpin(float speed) {
    m_spin = speed;
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::randSet(Ship* player) {
    randPos(player);
    randDir();
    randSpin();
    setDisplayed(true);
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::randDir() {
    m_direc.x = ((m_rand.rand() % 20) - 10 / 10.0f);
    m_direc.y = ((m_rand.rand() % 20) - 10 / 10.0f);
    m_direc *= (float)(m_rand.rand() % 20) + 1;
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::randPos(Ship* player) {
    // Selects a position within an area around the player
    float x = player->getPosition().x + (m_rand.rand() % 3000)-1500; // -1500x1500
    float y = player->getPosition().y + (m_rand.rand() % 2250)-750;  // -750x1500

    // If the position is on the screen push it out
    if ((player->getPosition().x - 960 < x && player->getPosition().x + 960 > x) && (player->getPosition().y - 540 < y && player->getPosition().y + 540 > y)) {
        if (y > player->getPosition().y) {
            y += 540;
        }
        else {
            if (y < player->getPosition().y) {
                y -= 540;
            }
            if (x < player->getPosition().x) {
                x -= 960;
            }
            else if (x > player->getPosition().x) {
                x += 960;
            }
        }
    }

    setPosition(x, y);
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::randSpin() {
    // Randomly between -1.0 and 1.0 revolutions a second
    m_spin = ((float)(m_rand.rand() % 20) - 10) / 10.0f;
}

/****************************************************************************************************
****************************************************************************************************/
void Asteroid::setupCollider() {
    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("circle", 5, 20, 17);

    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("circle", -10, 0, 21);

    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("circle", 10, -10, 25);
}