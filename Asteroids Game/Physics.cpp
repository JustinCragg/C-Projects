#include "Physics.h"
#include <Vector2.h>
#include <Functions.h>

#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
bool handleCollision(Sprite & a, Sprite & b) {
    float aRadius = 1;
    float bRadius = 1;

    // Checks whether the sprites length or width is greater
    if (a.getTexture()->getWidth() * a.getScale().x >= a.getTexture()->getHeight() * a.getScale().y) {
        aRadius = a.getTexture()->getWidth() * a.getScale().x;
    }
    else {
        aRadius = a.getTexture()->getWidth() * a.getScale().y;
    }
    if (b.getTexture()->getWidth() * b.getScale().x >= b.getTexture()->getHeight() * b.getScale().y) {
        bRadius = b.getTexture()->getWidth() * b.getScale().x;
    }
    else {
        bRadius = b.getTexture()->getHeight() * b.getScale().y;
    }

    // Only checks for collisions if it is possible for the objects to collide
    if (distanceBetweenSqr(a, b) < (aRadius + bRadius)*(aRadius + bRadius)) {
        for each (Collider* aCol in a.m_colliders) {
            // For every collider in the first sprite
            for each (Collider* bCol in b.m_colliders) {
                // For every collider in the second sprite
                // If aCol is a box
                if (aCol->m_type == "box") {
                    // If bCol is a box
                    if (bCol->m_type == "box") {
                        if (boxBoxCollision(*aCol, *bCol) == true) {
                            // Collision
                            return true;
                        }
                    }
                    // If bCol is a circle
                    else if (bCol->m_type == "circle") {
                        if (boxCircleCollision(*aCol, *bCol) == true) {
                            // Collision
                            return true;
                        }
                    }
                }
                // If aCol is a circle
                else if (aCol->m_type == "circle") {
                    // If bCol is a box
                    if (bCol->m_type == "box") {
                        if (boxCircleCollision(*bCol, *aCol) == true) {
                            // Collision
                            return true;
                        }
                    }
                    // If bCol is circle
                    else if (bCol->m_type == "circle") {
                        if (circleCircleCollision(*aCol, *bCol) == true) {
                            // Collision
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

/****************************************************************************************************
****************************************************************************************************/
bool boxBoxCollision(Collider& aCol, Collider& bCol) {
    float aX;
    float aY;
    math::Vector2 aHori;
    math::Vector2 aVert;

    // Gets the first collider's position
    aX = aCol.getWorldTransform()[2].x;
    aY = aCol.getWorldTransform()[2].y;

    // Gets the first collider's extents
    aHori[0] = aX - aCol.getWorldTransform().getScale().x / 2;
    aHori[1] = aX + aCol.getWorldTransform().getScale().x / 2;
    aVert[0] = aY - aCol.getWorldTransform().getScale().y / 2;
    aVert[1] = aY + aCol.getWorldTransform().getScale().y / 2;

    float bX;
    float bY;
    math::Vector2 bHori;
    math::Vector2 bVert;

    // Gets the second collider's position
    bX = aCol.getWorldTransform()[2].x;
    bY = aCol.getWorldTransform()[2].y;

    // Gets the second collider's extents
    bHori[0] = bX - bCol.getWorldTransform().getScale().x / 2;
    bHori[1] = bX + bCol.getWorldTransform().getScale().x / 2;
    bVert[0] = bY - bCol.getWorldTransform().getScale().y / 2;
    bVert[1] = bY + bCol.getWorldTransform().getScale().y / 2;

    // Horizontal Check
    if (aHori[1] >= bHori[0] && aHori[0] <= bHori[1]) {
        // Vertical Check
        if (aVert[1] >= bVert[0] && aVert[0] <= bVert[1]) {
            // Collision
            return true;
        }
    }
    // No collision
    return false;
}

/****************************************************************************************************
****************************************************************************************************/
bool boxCircleCollision(Collider& aCol, Collider& bCol) {
    float aX;
    float aY;
    math::Vector2 aHori;
    math::Vector2 aVert;

    // Gets the first collider's position
    aX = aCol.getWorldTransform()[2].x;
    aY = aCol.getWorldTransform()[2].y;

    // Gets the first collider's extents
    aHori[0] = aX - aCol.getWorldTransform().getScale().x / 2;
    aHori[1] = aX + aCol.getWorldTransform().getScale().x / 2;
    aVert[0] = aY - aCol.getWorldTransform().getScale().y / 2;
    aVert[1] = aY + aCol.getWorldTransform().getScale().y / 2;

    float bX;
    float bY;
    math::Vector2 bHori;
    math::Vector2 bVert;

    // Gets the second collider's position
    bX = bCol.getWorldTransform()[2].x;
    bY = bCol.getWorldTransform()[2].y;

    // Gets the closest point on the second collider's circumference
    math::Vector2 direc{ aX - bX, aY - bY };
    direc.normalise();
    direc *= bCol.getWorldTransform().getScale().x;
    direc += math::Vector2{ bX, bY };

    bHori[0] = direc.x;
    bHori[1] = direc.x;
    bVert[0] = direc.y;
    bVert[1] = direc.y;

    // Horizontal Check
    if (aHori[1] >= bHori[0] && aHori[0] <= bHori[1]) {
        // Vertical Check
        if (aVert[1] >= bVert[0] && aVert[0] <= bVert[1]) {
            // Collision
            return true;
        }
    }
    // No collision
    return false;
}

/****************************************************************************************************
****************************************************************************************************/
bool circleCircleCollision(Collider& aCol, Collider& bCol) {
    // Determines if the distance between the two colliders is small enough to collide
    if (distanceBetweenSqr(aCol, bCol) <= (aCol.getWorldTransform().getScale().x + bCol.getWorldTransform().getScale().x)*(aCol.getWorldTransform().getScale().x + bCol.getWorldTransform().getScale().x)) {
        // Collision
        return true;
    }
    // No collision
    return false;
}

/****************************************************************************************************
****************************************************************************************************/
void bounce(Asteroid& a, Asteroid& b, float deltaTime) {
    // Mass of 1 works for objects of the same mass in space
    float mass = 1;
    math::Vector3 normal{ b.getWorldTransform()[2] - a.getWorldTransform()[2] };
    normal.normalise();

    // Detemines asteroid's velocity in relation to each other
    math::Vector3 relVel = b.m_direc - a.m_direc;
    float velAlongNormal = relVel.dot(normal);
    if (velAlongNormal > 0) {
        return;
    }

    float j = -2 * velAlongNormal;
    j /= 1.0f / mass * 2;
    math::Vector3 impulse = normal * j;
    a.m_direc -= impulse * (1.0f / mass);
    b.m_direc += impulse * (1.0f / mass);
}

/****************************************************************************************************
****************************************************************************************************/
float distanceBetweenSqr(SceneObject& a, SceneObject& b) {
    float distance = 0;
    float x = abs(a.getWorldTransform()[2].x - b.getWorldTransform()[2].x);
    float y = abs(a.getWorldTransform()[2].y - b.getWorldTransform()[2].y);
    distance = x * x + y * y;
    return distance;
}

/****************************************************************************************************
****************************************************************************************************/
float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    else {
        return value;
    }
}