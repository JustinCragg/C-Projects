#pragma once
#include "SceneObject.h"

#include <string>

class Collider : public SceneObject {
public:
    using SceneObject::SceneObject;

    // Initialises the Collider values
    // Type can be ("box", "circle")
    // Width is not used for circles
    void init(std::string type, float x, float y, float length, float width = 0, float rotation = 0);

    // The type of collider
    std::string m_type = "";
};