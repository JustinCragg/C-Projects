#pragma once
#include "Need.h"

#include <Renderer2D.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <glm/vec4.hpp>

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    class SceneObject;
    class BaseAgent;

    // A struct which contains the base values for object templates
    struct EXPORT ObjectTemplate {
    public:
        std::string coreType = "None";
        std::string type = "None";
        glm::vec4 baseColour = glm::vec4(1);
    };

    // A template struct containing additional information for agents
    struct EXPORT AgentTemplate : ObjectTemplate {
    public:
        std::vector<Need*> baseNeeds = std::vector<Need*>();
        std::vector<std::vector<std::string>> baseLogicFunctions = std::vector<std::vector<std::string>>();
        std::vector<std::vector<std::string>> baseAppealFunctions = std::vector<std::vector<std::string>>();
    };

    // A template struct containing additional information for resources
    struct EXPORT ResourceTemplate : ObjectTemplate {
    public:
        float baseQuantity = 10;
        float baseRegrowth = 1;
        float baseGatherAmount = 1;
    };

    // A manager class which handles the initialisation, updating and drawing of various objects
    class EXPORT GameManager {
    public:
        // Standard Constructor
        GameManager();
        // Standard Destructor
        ~GameManager();

        // Uses multi-threading to call the update function of each object
        void update(float deltaTime);
        // Handles the drawing of each object
        void draw(aie::Renderer2D* renderer);
        // A visualisation of the agent's needs
        // Draws a bar for each of the agent's needs
        void drawStatBars(aie::Renderer2D* renderer, glm::vec2 windowSize, BaseAgent* agent);

        // Creates an object of the input type and returns a pointer to it
        SceneObject* createObject(std::string type);

        // Returns the nearest object of the type inputted, to the origin object
        SceneObject* getNearestObject(std::string type, SceneObject* origin);

        // Returns all the objects of the type specified
        std::vector<SceneObject*> getObjects(std::string type);

        // Removes and deletes an object
        void removeObject(SceneObject* object);

        // Reloads the scripts and updates all objects to use the updated scripts
        void reloadScripts();

    protected:
        // Returns the absolute path to the .exe
        std::string getExeFolderPath();
        // Splits a string into seperate words
        std::vector<std::string> splitString(std::string string);

        // Pairs up the list of commands with the corresponding functions
        void initScriptCommands();
        // Loads the scripts and creates templates for each of the object types
        void loadScripts();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Script Commands
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Handles the SET_TYPE command
        void onSetType(std::vector<std::string> line);
        // Handles the SET_COLOUR command
        void onSetColour(std::vector<std::string> line);

        // Handles the SET_NEEDS command
        void onSetNeeds(std::vector<std::string> line);
        // Handles the SET_DECAY command
        void onSetDecay(std::vector<std::string> line);
        // Handles the SET_FUNCTIONS command
        void onSetFunctions(std::vector<std::string> line);
        // Handles the SET_APPEAL command
        void onSetAppeal(std::vector<std::string> line);

        // Handles the SET_QUANTITY command
        void onSetQuantity(std::vector<std::string> line);
        // Handles the SET_REGROWTH command
        void onSetRegrowth(std::vector<std::string> line);
        // Handles the SET_GATHER_AMOUNT command
        void onSetGatherAmount(std::vector<std::string> line);

        // A list of commands which can be used in agent scripts
        std::unordered_map<std::string, std::function<void(const std::vector<std::string>)>> m_agentCommands;
        // A list of commands which can be used in resource scripts
        std::unordered_map<std::string, std::function<void(const std::vector<std::string>)>> m_resourceCommands;

        // The object templates
        std::vector<ObjectTemplate*> m_objectTemplates;

        // The objects in the world
        std::vector<SceneObject*> m_objects;
    };
}