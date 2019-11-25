#include "Bullet.h"

void Bullet::setupCollider() {
    m_colliders.push_back(new Collider{ this });
    m_colliders.back()->init("circle", 0, 0, 5);
}