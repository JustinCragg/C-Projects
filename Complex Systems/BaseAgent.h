#pragma once
#include "SceneObject.h"
#include "FuzzyLogic.h"
#include "Need.h"

#include <map>
#include <unordered_map>
#include <functional>

class BaseAgent : public SceneObject {
public:
    BaseAgent(glm::vec2 position, GameManager::ObjectType type);
    BaseAgent(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius = 15);
    virtual ~BaseAgent();

    virtual void update();
    virtual void draw(aie::Renderer2D* renderer);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Getters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<std::string> getNeedNames();

    // Returns the numerical value of the agent's hunger
	float getNeedValue(std::string need);

	float getNeedDecay(std::string need);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Misc.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void loadScript(std::string scriptPath) = 0;

protected:
	void init();

	std::vector<std::string> splitString(std::string string);

	using ScriptFunction = std::function<void(const std::vector<std::string>)>;
	std::unordered_map<std::string, ScriptFunction> m_allCommands;

	void initScriptCommands();
	void onSetNeeds(std::vector<std::string> line);
	void onSetDecay(std::vector<std::string> line);
	void onSetFunctions(std::vector<std::string> line);
	void onSetAppeal(std::vector<std::string> line);

	FuzzyLogic* m_logic;
	std::vector<std::vector<std::string>> m_needLevels;

	std::vector<Need*> m_needs;

	std::vector<std::vector<std::vector<std::string>>> m_appealSets;
};