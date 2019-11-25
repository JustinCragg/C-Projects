#include "GameManager.h"
#include "SceneObject.h"

#include <algorithm>

/************************************************************************************************************
************************************************************************************************************/
GameManager::GameManager() {

}

/************************************************************************************************************
************************************************************************************************************/
GameManager::~GameManager() {

}

/************************************************************************************************************
************************************************************************************************************/
void GameManager::update() {
    for each (SceneObject* prey in m_preys) {
        prey->update();
    }
	for each (SceneObject* water in m_waters) {
		water->update();
	}
	for each (SceneObject* food in m_foods) {
		food->update();
	}
	for each (SceneObject* shelter in m_shelters) {
		shelter->update();
	}
}

/************************************************************************************************************
************************************************************************************************************/
void GameManager::draw(aie::Renderer2D* renderer) {
	for each (SceneObject* prey in m_preys) {
		prey->draw(renderer);
	}
	for each (SceneObject* water in m_waters) {
		water->draw(renderer);
	}
	for each (SceneObject* food in m_foods) {
		food->draw(renderer);
	}
	for each (SceneObject* shelter in m_shelters) {
		shelter->draw(renderer);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void GameManager::addObject(SceneObject* object) {
	ObjectType type = object->getObjectType();
    switch (type) {
    case ObjectType::FOOD:
        m_foods.push_back(object);
        break;
    case ObjectType::SHELTER:
        m_shelters.push_back(object);
        break;
    case ObjectType::WATER:
        m_waters.push_back(object);
        break;
    case ObjectType::PREY:
        m_preys.push_back(object);
        break;
    case ObjectType::SIZE:
        break;
    default:
        break;
    }
	object->setGameManager(this);
}

/************************************************************************************************************
************************************************************************************************************/
struct lessThanDistance {
	inline bool operator() (const SceneObject* a, const SceneObject* b) {
		return a->m_distance < b->m_distance;
	}
};

/************************************************************************************************************
************************************************************************************************************/
SceneObject* GameManager::getNearestObject(ObjectType type, SceneObject* origin) {
	std::vector<SceneObject*> objects;
	switch (type) {
	case GameManager::FOOD:
		objects = getAllFood();
		break;
	case GameManager::SHELTER:
		objects = getAllShelter();
		break;
	case GameManager::WATER:
		objects = getAllWater();
		break;
	case GameManager::PREY:
		objects = getAllPrey();
		break;
	default:
		break;
	}

	for each (SceneObject* object in objects) {
		object->m_distance = glm::distance(object->getPosition(), origin->getPosition());
	}
	std::sort(objects.begin(), objects.end(), lessThanDistance());
	if (objects[0] == origin) {
		return objects[1];
	}
	else {
		return objects[0];
	}
}

/************************************************************************************************************
************************************************************************************************************/
SceneObject* GameManager::getMostAppealingObject(ObjectType type, SceneObject* origin) {
	return getNearestObject(type, origin);
}
