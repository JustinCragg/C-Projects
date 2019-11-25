#include "Ship.h"
#include "Bullet.h"
#include "Collider.h"

#include <iostream>
#include <string>

/****************************************************************************************************
****************************************************************************************************/
void Ship::shoot() {
    // If the cool down has finished
    if (m_shootCooldown.elapsed() > 0.3f || m_shootCooldown.running() == false) {
        // Shoot
        if (m_bulletsUsed == m_bulletPool.size()) {
            // If all bullets are in use add a new one
            m_bulletPool.push_back(new Bullet{getWorldRoot(), m_bulletPath, getPosition().x, getPosition().y });
            m_bulletPool[m_bulletPool.size() - 1]->setupCollider();
            m_bulletPool[m_bulletPool.size()-1]->lifeTime.start();
            m_bulletsUsed++;
        }
        else {
            for (int i = 0; i < m_bulletPool.size(); i++) {
                if (m_bulletPool[i]->getDisplayed() == false) {
                    // Reuses a bullet which is not in use
                    m_bulletPool[i]->setPosition(getPosition());
                    m_bulletPool[i]->setDisplayed(true);
                    m_bulletPool[i]->lifeTime.start();
                    m_bulletsUsed++;
                    break;
                }
            }
        }
        m_shootCooldown.start();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::addTurret(int maxLives) {
    // Adds a new life if the player is not already at max life
    if (m_turretPool.size() < maxLives) {
        m_turretPool.push_back(new Turret{ this, m_turretPath, 0, 0, 0.25f, 0.25f });
        m_turretPool[m_turretPool.size() - 1]->m_angle = (2 * PI / (float)maxLives) * m_turretPool.size();
        m_turretPool[m_turretPool.size() - 1]->addTurret();
    }
}

/****************************************************************************************************
****************************************************************************************************/
bool Ship::removeTurret() {
    int rand = (m_rand.rand() % m_turretPool.size());
    m_turretPool[rand] = m_turretPool.back();
    m_turretPool.pop_back();
    if (m_turretPool.size() <= 0) {
        // If there are no more lives left, return false
        return false;
    }
    else {
        return true;
    }
}

/****************************************************************************************************
****************************************************************************************************/
std::vector<Bullet*> Ship::getBulletPool() const {
    return m_bulletPool;
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::draw(aie::Renderer2D* renderer) {
    for (int i = 0; i < m_bulletPool.size(); i++) {
        // Draws bullets
        m_bulletPool[i]->draw(renderer);
    }
    // Draws ship
    Sprite::draw(renderer);
    // Draws lives
    for (int i = 0; i < m_turretPool.size(); i++) {
        m_turretPool[i]->draw(renderer);
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::bulletMove(float deltaTime, float speedOffset) {
    // Moves the bullets forward
    for (int i = 0; i < m_bulletPool.size(); i++) {
        if (m_bulletPool[i]->getDisplayed() == true) {
            m_bulletPool[i]->movePosition(0, (750 + speedOffset) * deltaTime);
            if (m_bulletPool[i]->lifeTime.elapsed() >= 2) {
                // After two seconds the bullet expires
                m_bulletPool[i]->setDisplayed(false);
                m_bulletsUsed--;
            }
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::turretMove(float deltaTime) {
    for (int i = 0; i < m_turretPool.size(); i++) {
        m_turretPool[i]->m_angle += 2 * deltaTime;
        m_turretPool[i]->setPosition(60 * cos(m_turretPool[i]->m_angle), 60 * sin(m_turretPool[i]->m_angle));
        m_turretPool[i]->turretMove(deltaTime);
    }
}

/****************************************************************************************************
****************************************************************************************************/
int Ship::getLives() const {
    return (int)m_turretPool.size();
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::setPaths(const std::string & bulletPath, const std::string & turretPath) {
    m_bulletPath = bulletPath;
    m_turretPath = turretPath;
}

/****************************************************************************************************
****************************************************************************************************/
void Ship::setupCollider() {
    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("box", 0, -5, 90, 20);
    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("box", 0, 0, 40, 75);
}