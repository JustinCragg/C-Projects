#include "stdafx.h"
#include "GameManager.h"
#include "SceneObject.h"
#include "BaseAgent.h"
#include "BaseResource.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <functional>
#include <filesystem>
#include <Windows.h>
#include <thread>

namespace flai {
    /************************************************************************************************************
    ************************************************************************************************************/
    GameManager::GameManager() {
        initScriptCommands();
        loadScripts();
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    GameManager::~GameManager() {
        for (SceneObject* object : m_objects) {
            delete object;
        }
        for (ObjectTemplate* temp : m_objectTemplates) {
            delete temp;
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    std::vector<std::string> GameManager::splitString(std::string string) {
        std::istringstream iss(string);
        return std::vector<std::string>(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>());
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void GameManager::initScriptCommands() {
        m_agentCommands["SET_TYPE"] = [this](const std::vector<std::string> line) { onSetType(line); };
        m_agentCommands["SET_COLOUR"] = [this](const std::vector<std::string> line) { onSetColour(line); };
        m_agentCommands["SET_NEEDS"] = [this](const std::vector<std::string> line) { onSetNeeds(line); };
        m_agentCommands["SET_DECAY"] = [this](const std::vector<std::string> line) { onSetDecay(line); };
        m_agentCommands["SET_FUNCTIONS"] = [this](const std::vector<std::string> line) { onSetFunctions(line); };
        m_agentCommands["SET_APPEAL"] = [this](const std::vector<std::string> line) { onSetAppeal(line); };

        m_resourceCommands["SET_TYPE"] = [this](const std::vector<std::string> line) { onSetType(line); };
        m_resourceCommands["SET_COLOUR"] = [this](const std::vector<std::string> line) { onSetColour(line); };
        m_resourceCommands["SET_QUANTITY"] = [this](const std::vector<std::string> line) { onSetQuantity(line); };
        m_resourceCommands["SET_REGROWTH"] = [this](const std::vector<std::string> line) { onSetRegrowth(line); };
        m_resourceCommands["SET_GATHER_AMOUNT"] = [this](const std::vector<std::string> line) { onSetGatherAmount(line); };
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    std::string GameManager::getExeFolderPath() {
        // Obtain the path to the folder that contains the executable.
        wchar_t buffer[1000];
        GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof(buffer) - 1);

        char ch[1000];
        char defChar = ' ';
        WideCharToMultiByte(CP_ACP, 0, buffer, -1, ch, 1000, &defChar, NULL);

        for (int i = 0; i < 1000; i++) {
            if (ch[i] == '.' && ch[i + 1] == 'e' && ch[i + 2] == 'x' && ch[i + 3] == 'e') {
                ch[i + 4] = 0;
                for (int j = i + 4; j >= 0; j--) {
                    if (ch[j] == '/') {
                        // cut off everything after
                        ch[j + 1] = 0;
                        return std::string(ch);
                    }
                    else if (ch[j] == '\\') {
                        // cut off everything after
                        ch[j + 1] = 0;
                        return std::string(ch);
                    }
                }
            }
        }

        return std::string(ch);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::loadScripts() {
        std::string path = getExeFolderPath() + "/scripts/agents/";
        for (const auto& entry : std::experimental::filesystem::directory_iterator(path)) {
            std::ifstream scriptFile(entry.path(), std::ios::in);
            std::string line;
            m_objectTemplates.push_back(new AgentTemplate());
            m_objectTemplates.back()->coreType = "Agent";
            while (scriptFile.good() == true) {
                if (std::getline(scriptFile, line)) {
                    if (line.size() != 0) {
                        // execute line
                        std::vector<std::string> words = splitString(line);
                        std::string command = words[0];
                        if (command[0] == '/' && command[1] == '/') {
                            continue;
                        }
                        auto iter = m_agentCommands.find(command);
                        if (iter != m_agentCommands.end()) {
                            iter->second(words);
                        }
                        else {
                            std::cout << "Attempted to run invalid command '" << command << "'\n";
                        }
                    }
                }
            }
        }
        path = getExeFolderPath() + "/scripts/resources/";
        for (const auto& entry : std::experimental::filesystem::directory_iterator(path)) {
            std::ifstream scriptFile(entry.path(), std::ios::in);
            std::string line;
            m_objectTemplates.push_back(new ResourceTemplate());
            m_objectTemplates.back()->coreType = "Resource";
            while (scriptFile.good() == true) {
                if (std::getline(scriptFile, line)) {
                    if (line.size() != 0) {
                        // execute line
                        std::vector<std::string> words = splitString(line);
                        std::string command = words[0];
                        auto iter = m_resourceCommands.find(command);
                        if (iter != m_resourceCommands.end()) {
                            iter->second(words);
                        }
                        else {
                            std::cout << "Attempted to run invalid command '" << command << "'\n";
                        }
                    }
                }
            }
        }
        assert(m_objectTemplates.size() != 0);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::reloadScripts() {
        std::string path = getExeFolderPath() + "/scripts/agents/";
        for (const auto& entry : std::experimental::filesystem::directory_iterator(path)) {
            std::ifstream scriptFile(entry.path(), std::ios::in);
            std::string line;

            std::getline(scriptFile, line);
            std::vector<std::string> tempWords = splitString(line);

            // Checks if the template has already been made
            assert(tempWords[0] == "SET_TYPE");
            for (int i = 0; i < m_objectTemplates.size(); i++) {
                if (tempWords[1] == m_objectTemplates[i]->type) {
                    m_objectTemplates.erase(m_objectTemplates.begin() + i);
                    break;
                }
            }

            m_objectTemplates.push_back(new AgentTemplate());
            m_objectTemplates.back()->coreType = "Agent";
            auto iter = m_agentCommands.find(tempWords[0]);
            if (iter != m_agentCommands.end()) {
                iter->second(tempWords);
            }

            while (scriptFile.good() == true) {
                if (std::getline(scriptFile, line)) {
                    if (line.size() != 0) {
                        // execute line
                        std::vector<std::string> words = splitString(line);
                        std::string command = words[0];
                        if (command[0] == '/' && command[1] == '/') {
                            continue;
                        }
                        auto iter = m_agentCommands.find(command);
                        if (iter != m_agentCommands.end()) {
                            iter->second(words);
                        }
                        else {
                            std::cout << "Attempted to run invalid command '" << command << "'\n";
                        }
                    }
                }
            }
        }
        path = getExeFolderPath() + "/scripts/resources/";
        for (const auto& entry : std::experimental::filesystem::directory_iterator(path)) {
            std::ifstream scriptFile(entry.path(), std::ios::in);
            std::string line;

            std::getline(scriptFile, line);
            std::vector<std::string> tempWords = splitString(line);

            // Checks if the template has already been made
            assert(tempWords[0] == "SET_TYPE");
            for (int i = 0; i < m_objectTemplates.size(); i++) {
                if (tempWords[1] == m_objectTemplates[i]->type) {
                    m_objectTemplates.erase(m_objectTemplates.begin() + i);
                    break;
                }
            }

            m_objectTemplates.push_back(new ResourceTemplate());
            m_objectTemplates.back()->coreType = "Resource";
            auto iter = m_agentCommands.find(tempWords[0]);
            if (iter != m_agentCommands.end()) {
                iter->second(tempWords);
            }

            while (scriptFile.good() == true) {
                if (std::getline(scriptFile, line)) {
                    if (line.size() != 0) {
                        // execute line
                        std::vector<std::string> words = splitString(line);
                        std::string command = words[0];
                        auto iter = m_resourceCommands.find(command);
                        if (iter != m_resourceCommands.end()) {
                            iter->second(words);
                        }
                        else {
                            std::cout << "Attempted to run invalid command '" << command << "'\n";
                        }
                    }
                }
            }
        }

        // Updates the objects to use the new templates
        for (SceneObject* object : m_objects) {
            for (ObjectTemplate* temp : m_objectTemplates) {
                if (object->getType() == temp->type) {
                    BaseAgent* agent = dynamic_cast<BaseAgent*>(object);
                    BaseResource* resource = dynamic_cast<BaseResource*>(object);
                    if (agent != nullptr) {
                        agent->updateScriptValues((AgentTemplate*)temp);
                    }
                    else if (resource != nullptr) {
                        resource->updateScriptValues((ResourceTemplate*)temp);
                    }
                    continue;
                }
            }
        }
        assert(m_objectTemplates.size() != 0);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetType(std::vector<std::string> line) {
        assert(line.size() == 2);
        ObjectTemplate* agent = m_objectTemplates.back();
        agent->type = line[1];
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetColour(std::vector<std::string> line) {
        assert(line.size() == 4);
        ObjectTemplate* agent = m_objectTemplates.back();
        for (int i = 0; i < 3; i++) {
            agent->baseColour[i] = (float)atof(line[i + 1].c_str());
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetNeeds(std::vector<std::string> line) {
        assert(line.size() > 1);
        AgentTemplate* agent = (AgentTemplate*)m_objectTemplates.back();
        for (int i = 1; i < line.size(); i++) {
            agent->baseNeeds.push_back(new Need(line[i]));
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetDecay(std::vector<std::string> line) {
        assert(line.size() == 3);
        AgentTemplate* agent = (AgentTemplate*)m_objectTemplates.back();
        int needIndex = -1;
        for (int i = 0; i < agent->baseNeeds.size(); i++) {
            if (agent->baseNeeds[i]->m_name == line[1]) {
                needIndex = i;
                break;
            }
        }
        assert(needIndex != -1);
        agent->baseNeeds[needIndex]->m_decay = (float)atof(line[2].c_str());
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetFunctions(std::vector<std::string> line) {
        assert(line.size() >= 6);
        AgentTemplate* agent = (AgentTemplate*)m_objectTemplates.back();
        int index = -1;
        if (line[1] == "distance") {
            index = (int)agent->baseNeeds.size();
        }
        else if (line[1] == "appeal") {
            index = (int)agent->baseNeeds.size() + 1;
        }
        else {
            for (int i = 0; i < agent->baseNeeds.size(); i++) {
                if (agent->baseNeeds[i]->m_name == line[1]) {
                    index = i;
                    break;
                }
            }
        }
        assert(index != -1);
        agent->baseLogicFunctions.push_back(line);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetAppeal(std::vector<std::string> line) {
        assert(line.size() > 3);
        AgentTemplate* agent = (AgentTemplate*)m_objectTemplates.back();
        int needIndex = -1;
        for (int i = 0; i < agent->baseNeeds.size(); i++) {
            if (agent->baseNeeds[i]->m_name == line[1]) {
                needIndex = i;
                break;
            }
        }
        assert(needIndex != -1);
        int index = -1;
        for (int i = 0; i < agent->baseLogicFunctions.size(); i++) {
            if (agent->baseLogicFunctions[i][3] == line[2]) {
                index = i;
                break;
            }
        }
        assert(index != -1);
        agent->baseAppealFunctions.push_back(line);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetQuantity(std::vector<std::string> line) {
        assert(line.size() == 2);
        ResourceTemplate* agent = (ResourceTemplate*)m_objectTemplates.back();
        agent->baseQuantity = (float)atof(line[1].c_str());
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetRegrowth(std::vector<std::string> line) {
        assert(line.size() == 2);
        ResourceTemplate* agent = (ResourceTemplate*)m_objectTemplates.back();
        agent->baseRegrowth = (float)atof(line[1].c_str());
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::onSetGatherAmount(std::vector<std::string> line) {
        assert(line.size() == 2);
        ResourceTemplate* agent = (ResourceTemplate*)m_objectTemplates.back();
        agent->baseGatherAmount = (float)atof(line[1].c_str());
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::update(float deltaTime) {
        std::vector<std::thread> threads;
        std::vector<SceneObject*> objects = m_objects;
        int vectorSize = (int)objects.size();
        int numThreads = std::thread::hardware_concurrency();
        int chunkLength = vectorSize / numThreads;
        int count = 0;

        for (int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(
                [&](int low, int high) {
                for (int j = low; j < high; j++) {
                    objects[j]->update(deltaTime);
                }
            }, i * chunkLength + count, glm::min((i + 1) * chunkLength + (count + 1), vectorSize)));
            if (numThreads*chunkLength + count <= vectorSize) {
                count++;
            }
        }

        for (std::thread& thread : threads) {
            thread.join();
        }

        std::vector<int> removeIndex = std::vector<int>();
        for (int i = 0; i < m_objects.size(); i++) {
            BaseAgent* agent = dynamic_cast<BaseAgent*>(m_objects[i]);
            if (agent != nullptr) {
                if (agent->getHealth() <= 0) {
                    removeIndex.push_back(i);
                }
            }
        }
        for (int index : removeIndex) {
            removeObject(m_objects[index]);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::drawStatBars(aie::Renderer2D* renderer, glm::vec2 window, BaseAgent* agent) {
        for (int i = 0; i < agent->getNeeds().size(); i++) {
            renderer->setRenderColour(0.6f, 0.6f, 0.6f);
            renderer->drawBox(55, window.x - 10 - i * 15.0f, 100, 10);
            glm::vec4 colour = getObjects(agent->getNeeds()[i]->m_name)[0]->getColour();
            float value = agent->getNeeds()[i]->m_value * 100.0f;
            renderer->setRenderColour(colour.r, colour.g, colour.b);
            renderer->drawBox(55 - (100 - value) * 0.5f, window.y - 10 - i * 15.0f, value, 10);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::draw(aie::Renderer2D* renderer) {
        for (SceneObject* object : m_objects) {
            object->draw(renderer);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    SceneObject* GameManager::createObject(std::string type) {
        SceneObject* object = nullptr;

        for (ObjectTemplate* objectTemplate : m_objectTemplates) {
            if (objectTemplate->type == type) {
                if (objectTemplate->coreType == "Agent") {
                    object = new BaseAgent();
                    dynamic_cast<BaseAgent*>(object)->setScriptValues((AgentTemplate*)objectTemplate);
                    object->setGameManager(this);
                    m_objects.push_back(object);
                }
                else if (objectTemplate->coreType == "Resource") {
                    object = new BaseResource();
                    dynamic_cast<BaseResource*>(object)->setScriptValues((ResourceTemplate*)objectTemplate);
                    object->setGameManager(this);
                    m_objects.push_back(object);
                }
                break;
            }
        }
        assert(object != nullptr);
        return object;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    struct lessThanDistance {
        inline bool operator() (const std::pair<SceneObject*, float> a, const std::pair<SceneObject*, float> b) {
            return a.second < b.second;
        }
    };

    /************************************************************************************************************
    ************************************************************************************************************/
    SceneObject* GameManager::getNearestObject(std::string type, SceneObject* origin) {
        std::vector<std::pair<SceneObject*, float>> distances;
        for (SceneObject* object : m_objects) {
            if (object->getType() == type) {
                distances.push_back(std::pair<SceneObject*, float>(object, glm::distance(object->getPosition(), origin->getPosition())));
            }
        }

        std::sort(distances.begin(), distances.end(), lessThanDistance());
        if (distances[0].first == origin) {
            return distances[1].first;
        }
        else {
            return distances[0].first;
        }
        return nullptr;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    std::vector<SceneObject*> GameManager::getObjects(std::string type) {
        std::vector<SceneObject*> objects = std::vector<SceneObject*>();
        for (SceneObject* object : m_objects) {
            if (object->getType() == type) {
                objects.push_back(object);
            }
        }
        return objects;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void GameManager::removeObject(SceneObject* object) {
        for (int i = 0; i < m_objects.size(); i++) {
            if (object == m_objects[i]) {
                delete m_objects[i];
                m_objects.erase(m_objects.begin() + i);
                return;
            }
        }
    }
}