#include "Collider.h"

#include <string>

/****************************************************************************************************
****************************************************************************************************/
void Collider::init(std::string type, float x, float y, float length, float width, float rotation) {
    m_type = type;
    setPosition(x, y);
    if (rotation != 0) {
        setRotation(rotation);
    }
    setScale(length, width);
}