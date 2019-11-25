#pragma once
#include "BaseResource.h"

class Food : public BaseResource {
public:
    Food(glm::vec2 position);
    Food(glm::vec2 position, glm::vec4 colour, float radius = 15);
    virtual ~Food();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

protected:
	virtual float determineAbundance() { return 0; }
};