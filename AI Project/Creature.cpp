#include "Creature.h"
#include "Sprite.h"
#include "Transform.h"
#include "NavAgent.h"

/****************************************************************************************************
****************************************************************************************************/
Creature::Creature() {
    // Due to the order in which functions are called, the init() has to be called after the child's constructor
    //init();
    m_health = (float)(m_randGen.rand() % 50);
    m_health += 50;
    m_hunger = (float)(m_randGen.rand() % 50);
    m_hunger += 50;
}

/****************************************************************************************************
****************************************************************************************************/
void Creature::init(std::string exePath, Transform* transformParent, NavMap* navMap) {
    addComponent(new Sprite{ this, exePath + m_filePath });
    addComponent(new Transform{ this, transformParent });
    if (navMap != nullptr) {
        // Can't create a navAgent without the navMap
        addComponent(new NavAgent{ this, navMap });
    }

    m_randGen.setSeedNow();
}

/****************************************************************************************************
****************************************************************************************************/
Creature::~Creature() {

}