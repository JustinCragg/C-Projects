#pragma once
#include "BaseResource.h"

class Shelter : public BaseResource {
public:
    Shelter(glm::vec2 position);
    Shelter(glm::vec2 position, glm::vec4 colour, float radius = 15);
    virtual ~Shelter();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

protected:
	virtual float determineAbundance() { return 0; }
};