#include "Food.h"
#include "StateMachine.h"
#include "SMWander.h"

/****************************************************************************************************
****************************************************************************************************/
Food::Food(std::string exePath, Transform* transformParent) {
    init(exePath, transformParent);
}

/****************************************************************************************************
****************************************************************************************************/
void Food::init(std::string exePath, Transform* transformParent) {
    m_filePath = "/textures/food.png";
    Creature::init(exePath, transformParent);
    addComponent(new StateMachine{ this, new SMWander });

    m_health = 10;
}

/****************************************************************************************************
****************************************************************************************************/
Food::~Food() {

}

/****************************************************************************************************
****************************************************************************************************/
void Food::update(std::vector<Creature*> allPred, std::vector<Creature*> allPrey, std::vector<Creature*> allFood) {
    ((StateMachine*)getComponent(comp::StateMachine))->update(allPred, allPrey, allFood);
}