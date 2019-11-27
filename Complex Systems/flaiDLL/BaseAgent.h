#pragma once
#include "SceneObject.h"
#include "FuzzyLogic.h"
#include "Need.h"

#include <map>
#include <unordered_map>
#include <functional>

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    // Base class for all agents
    // Handles the decision making and the needs of the agent
    class EXPORT BaseAgent : public SceneObject {
    public:
        // Standard Constructor
        BaseAgent();
        // Standard Destructor
        virtual ~BaseAgent();

        // Initialises the agent
        // Defaults colour to the script's colour and radius to 15
        void init(glm::vec2 position);
        // Initialises the agent
        void init(glm::vec2 position, glm::vec4 colour, float radius);

        // Determines what needs are important and moves towards them
        // This function is called by the gameManager is used called using multi-threading
        // THIS FUNCTION CANNOT CHANGE VALUES IN O0THER OBJECTS
        virtual void update(float deltaTime);
        // Draws the agent
        virtual void draw(aie::Renderer2D* renderer);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Getters
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Returns the names of each of this agent's needs
        std::vector<std::string> getNeedNames();

        // Returns the value for the corresponding need
        float getNeedValue(std::string need);
        // Returns the rate at which the correspnding need decays at
        float getNeedDecay(std::string need);

        // Returns this agent's needs
        std::vector<Need*> getNeeds() { return m_needs; }

        // Returns the health of this agent
        float getHealth() { return m_health; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Setters
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Initialises this resource's values according to the template given
        void setScriptValues(AgentTemplate* agentTemplate);
        // Updates this resource's values according to the template given
        void updateScriptValues(AgentTemplate* agentTemplate);

        // Sets the corresponding need value
        void setNeedValue(std::string type, float value);

        // Sets the base speed this agent moves at
        void setSpeed(float speed) { m_speed = speed; }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Misc.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Increases the corresponding need value
        void addValueToNeed(std::string type, float value);

        // Reduces the health of the agent by the damage
        void takeDamage(float damage) { m_health -= damage; }

    protected:
        // Assigns a new need function used by this agent
        void assignFunctions(std::vector<std::string> line);
        // Assigns a new appeal equation used by this agent
        void assignAppeal(std::vector<std::string> line);

        // Determines the appeal of a need according to the associated appeal equation
        std::pair<float, int> calcLogic(int needIndex, std::vector<std::vector<float>> values, std::vector<std::string> logics);
        // Converts a set of desire values into a single floating point value
        float defuzzify(std::vector<float> values);
        // Returns a single number for the desire of a need
        std::vector<std::pair<std::string, float>> getAppeals();

        // The fuzzy logic engine
        FuzzyLogic* m_logic;

        // The needs this agent has
        std::vector<Need*> m_needs;

        // The agent's health
        float m_health = 1;
        // The damage this agent doesl
        float m_damage = 0.1f;

        // The set of appeal equation for each need this agent uses
        std::vector<std::vector<std::vector<std::string>>> m_appealSets;
    };
}