#pragma once
#include <Renderer2D.h>
#include <vector>

class SceneObject;

class GameManager {
public:
    enum ObjectType { FOOD, SHELTER, WATER, PREY, SIZE };

    GameManager();
    ~GameManager();

    void update();
    void draw(aie::Renderer2D* renderer);

    void addObject(SceneObject* object);

	SceneObject* getNearestObject(ObjectType type, SceneObject* origin);
	SceneObject* getMostAppealingObject(ObjectType type, SceneObject* origin);

    std::vector<SceneObject*> getAllPrey() { return m_preys; }
    std::vector<SceneObject*> getAllWater() { return m_waters; }
    std::vector<SceneObject*> getAllFood() { return m_foods; }
    std::vector<SceneObject*> getAllShelter() { return m_shelters; }

protected:
    std::vector<SceneObject*> m_preys;
    std::vector<SceneObject*> m_waters;
    std::vector<SceneObject*> m_foods;
    std::vector<SceneObject*> m_shelters;
};