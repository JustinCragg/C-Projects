#pragma once
#include "BaseResource.h"

class Water : public BaseResource {
public:
    Water(glm::vec2 position);
    Water(glm::vec2 position, glm::vec4 colour, float radius = 15);
    virtual ~Water();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

protected:
	virtual float determineAbundance() { return 0; }
};