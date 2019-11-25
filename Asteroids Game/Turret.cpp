#include "Turret.h"
#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
void Turret::addTurret() {
    // Adds three orbiting balls around the ball
    for (int i = (int)m_turretPool.size(); i < NUM_BALLS; i++) {
        m_turretPool.push_back(new Turret{ this, m_filePath, 0, 0, 0.25f, 0.25f });
        // Sets its position evenly spaced out along the orbit
        m_turretPool[m_turretPool.size() - 1]->m_angle = (2 * PI / (float)NUM_BALLS) * m_turretPool.size();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Turret::turretMove(float deltaTime) {
    // Moves the orbiting balls along their orbit
    for (int i = 0; i < m_turretPool.size(); i++) {
        m_turretPool[i]->m_angle -= 5 * deltaTime;
        // bob is as it is because the numbers worked
        float bob = (float)((int)(m_angle*10) % 200)-100;
        m_turretPool[i]->setPosition(bob *cos(m_turretPool[i]->m_angle), bob * sin(m_turretPool[i]->m_angle));
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Turret::draw(aie::Renderer2D* renderer) {
    Sprite::draw(renderer);
    for (int i = 0; i < m_turretPool.size(); i++) {
        m_turretPool[i]->draw(renderer);
    }
}