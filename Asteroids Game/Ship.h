#pragma once
#include "Sprite.h"
#include "Bullet.h"
#include "Turret.h"
#include "Collider.h"

#include <coreTimer.h>

#include <vector>

//const int MAX_PLAYER_LIVES = 20;

class Ship : public Sprite {
public:
    using Sprite::Sprite;

    // Sets the absolute paths for future bullets and lives
    void setPaths(const std::string& bulletPath, const std::string& turretPath);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Lives
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Adds a life - up to the max
    void addTurret(int maxLives);
    // Removes a life - removing the last life will return false
    bool removeTurret();
    // Moves the lives around the ship
    void turretMove(float deltaTime);
    // Returns the number of lives remaining
    int getLives() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Shooting
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // If the cooldown has finished will create a bullet
    void shoot();
    // Returns the pool of bullets currently stored
    std::vector<Bullet*> getBulletPool() const;
    // Moves the bullets which are in use
    void bulletMove(float deltaTime, float speedOffset);
    // The number of bullets currently in use
    int m_bulletsUsed = 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Collider
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Creates the colliders for the Ship
    void setupCollider();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Drawing
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Draws the bullets which are in use, followed by the ship sprite, and last the lives
    virtual void draw(aie::Renderer2D* renderer);

protected:
    // The pool of lives in use
    std::vector<Turret*> m_turretPool;
    // The absolute path for the life texture
    std::string m_turretPath;

    // The pool of bullets in use
    std::vector<Bullet*> m_bulletPool;
    // The absolute path for the bullet texture
    std::string m_bulletPath;
    // The timer used to determine the cooldown when shooting
    core::Timer m_shootCooldown;
};