#pragma once
#include "BaseAgent.h"

class Prey : public BaseAgent {
public:
    Prey(glm::vec2 position, std::string scriptPath);
    Prey(glm::vec2 position, std::string scriptPath, glm::vec4 colour, float radius = 15);
    virtual ~Prey();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

protected:
	void init(std::string scriptPath);
	float calcLogic(int needIndex, std::vector<std::vector<float>> values, std::vector<std::string> logics);
	virtual void loadScript(std::string scriptPath);

	std::vector<std::pair<std::string, float>> getDesires();
};