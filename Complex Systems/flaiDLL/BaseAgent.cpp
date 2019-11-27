#include "stdafx.h"
#include "BaseAgent.h"
#include "BaseResource.h"

#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>

namespace flai {
    /************************************************************************************************************
    ************************************************************************************************************/
    BaseAgent::BaseAgent() : SceneObject() {
        m_logic = new FuzzyLogic();
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    BaseAgent::~BaseAgent() {
        delete m_logic;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::init(glm::vec2 position) {
        m_position = position;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::init(glm::vec2 position, glm::vec4 colour, float radius) {
        m_position = position;
        m_colour = colour;
        m_radius = radius;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    std::vector<std::string> BaseAgent::getNeedNames() {
        std::vector<std::string> output = std::vector<std::string>();
        for (Need* need : m_needs) {
            output.push_back(need->m_name);
        }
        return output;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float BaseAgent::getNeedValue(std::string type) {
        for (Need* need : m_needs) {
            if (need->m_name == type) {
                return need->m_value;
            }
        }
        return 0;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float BaseAgent::getNeedDecay(std::string type) {
        for (Need* need : m_needs) {
            if (need->m_name == type) {
                return need->m_decay;
            }
        }
        return 0;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::setScriptValues(AgentTemplate* agentTemplate) {
        m_objectType = agentTemplate->type;
        m_colour = agentTemplate->baseColour;
        // Set needs
        for (int i = 0; i < (int)agentTemplate->baseNeeds.size(); i++) {
            m_needs.push_back(new Need(agentTemplate->baseNeeds[i]->m_name));
            m_needs.back()->m_colour = agentTemplate->baseNeeds[i]->m_colour;
            m_needs.back()->m_decay = agentTemplate->baseNeeds[i]->m_decay;
            m_needs.back()->m_value = agentTemplate->baseNeeds[i]->m_value;
        }

        for (int i = 0; i < (int)m_needs.size(); i++) {
            m_appealSets.push_back(std::vector<std::vector<std::string>>());
            m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        }
        m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        // Set functions for determining needs
        for (std::vector<std::string> line : agentTemplate->baseLogicFunctions) {
            assignFunctions(line);
        }
        // Set functions for determining the appeal of needs
        for (std::vector<std::string> line : agentTemplate->baseAppealFunctions) {
            assignAppeal(line);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::updateScriptValues(AgentTemplate* agentTemplate) {
        m_colour = agentTemplate->baseColour;
        // Update needs
        for (Need* scriptNeed : agentTemplate->baseNeeds) {
            bool good = false;
            for (Need* need : m_needs) {
                // Updates old need
                if (scriptNeed->m_name == need->m_name) {
                    need->m_colour = scriptNeed->m_colour;
                    need->m_decay = scriptNeed->m_decay;
                    good = true;
                    continue;
                }
            }
            // Creates new need
            if (good == false) {
                m_needs.push_back(new Need(scriptNeed->m_name));
                m_needs.back()->m_colour = scriptNeed->m_colour;
                m_needs.back()->m_decay = scriptNeed->m_decay;
                m_needs.back()->m_value = scriptNeed->m_value;
            }
        }

        // Clear old functions
        m_logic->m_logicFunctions.clear();
        m_appealSets.clear();

        for (int i = 0; i < (int)m_needs.size(); i++) {
            m_appealSets.push_back(std::vector<std::vector<std::string>>());
            m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        }
        m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
        // Set functions for determining needs
        for (std::vector<std::string> line : agentTemplate->baseLogicFunctions) {
            assignFunctions(line);
        }
        // Set functions for determining the appeal of needs
        for (std::vector<std::string> line : agentTemplate->baseAppealFunctions) {
            assignAppeal(line);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::setNeedValue(std::string type, float value) {
        for (Need* need : m_needs) {
            if (need->m_name == type) {
                need->m_value = value;
                return;
            }
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::addValueToNeed(std::string type, float value) {
        for (Need* need : m_needs) {
            if (need->m_name == type) {
                need->m_value += value;
                return;
            }
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::assignFunctions(std::vector<std::string> line) {
        assert(line.size() >= 6);
        int index = -1;
        if (line[1] == "distance") {
            index = (int)m_needs.size();
        }
        else if (line[1] == "appeal") {
            index = (int)m_needs.size() + 1;
        }
        else {
            for (int i = 0; i < (int)m_needs.size(); i++) {
                if (m_needs[i]->m_name == line[1]) {
                    index = i;
                    m_appealSets[index].push_back(std::vector<std::string>());
                    break;
                }
            }
        }
        assert(index != -1);
        if (line[2] == "LEFT_SHOULDER") {
            m_logic->m_logicFunctions[index].push_back(new LeftShoulderFunction(line[3], (float)atof(line[4].c_str()), (float)atof(line[5].c_str())));
        }
        else if (line[2] == "TRIANGLE") {
            m_logic->m_logicFunctions[index].push_back(new TriangleFunction(line[3], (float)atof(line[4].c_str()), (float)atof(line[5].c_str()), (float)atof(line[6].c_str())));
        }
        else if (line[2] == "TRAPEZOID") {
            m_logic->m_logicFunctions[index].push_back(new TrapezoidFunction(line[3], (float)atof(line[4].c_str()), (float)atof(line[5].c_str()), (float)atof(line[6].c_str()), (float)atof(line[7].c_str())));
        }
        else if (line[2] == "RIGHT_SHOULDER") {
            m_logic->m_logicFunctions[index].push_back(new RightShoulderFunction(line[3], (float)atof(line[4].c_str()), (float)atof(line[5].c_str())));
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::assignAppeal(std::vector<std::string> line) {
        assert(line.size() > 3);
        int needIndex = -1;
        for (int i = 0; i < (int)m_needs.size(); i++) {
            if (m_needs[i]->m_name == line[1]) {
                needIndex = i;
                break;
            }
        }
        assert(needIndex != -1);
        int index = -1;
        for (int i = 0; i < (int)m_logic->m_logicFunctions[m_needs.size() + 1].size(); i++) {
            if (m_logic->m_logicFunctions[m_needs.size() + 1][i]->_id == line[2]) {
                index = i;
                break;
            }
        }
        assert(index != -1);
        for (int i = 3; i < (int)line.size(); i++) {
            m_appealSets[needIndex][index].push_back(line[i]);
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    std::pair<float, int> BaseAgent::calcLogic(int needIndex, std::vector<std::vector<float>> values, std::vector<std::string> logics) {
        if (logics[0] == "AND") {
            std::vector<std::string> temp = logics;
            temp.erase(temp.begin());

            std::pair<float, int> resultA = calcLogic(needIndex, values, temp);
            float a = resultA.first;
            temp.erase(temp.begin(), temp.begin() + resultA.second);

            std::pair<float, int> resultB = calcLogic(needIndex, values, temp);
            float b = resultB.first;

            return std::pair<float, int>(FuzzyLogic::AND(a, b), resultA.second + resultB.second + 1);
        }
        else if (logics[0] == "OR") {
            std::vector<std::string> temp = logics;
            temp.erase(temp.begin());

            std::pair<float, int> resultA = calcLogic(needIndex, values, temp);
            float a = resultA.first;
            temp.erase(temp.begin(), temp.begin() + resultA.second);

            std::pair<float, int> resultB = calcLogic(needIndex, values, temp);
            float b = resultB.first;

            return std::pair<float, int>(FuzzyLogic::OR(a, b), resultA.second + resultB.second + 1);
        }
        else if (logics[0] == "NOT") {
            std::vector<std::string> temp = logics;
            temp.erase(temp.begin());

            std::pair<float, int> resultA = calcLogic(needIndex, values, temp);
            float a = resultA.first;

            return std::pair<float, int>(FuzzyLogic::NOT(a), resultA.second + 1);
        }
        else {
            int index = -1;
            for (int i = 0; i < (int)m_logic->m_logicFunctions[needIndex].size(); i++) {
                if (m_logic->m_logicFunctions[needIndex][i]->_id == logics[0]) {
                    return std::pair<float, int>(values[0][i], 1);
                }
            }
            for (int i = 0; i < (int)m_logic->m_logicFunctions[m_needs.size()].size(); i++) {
                if (m_logic->m_logicFunctions[m_needs.size()][i]->_id == logics[0]) {
                    return std::pair<float, int>(values[1][i], 1);
                }
            }
        }
        return std::pair<float, int>(0.0f, 0);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float BaseAgent::defuzzify(std::vector<float> values) {
        float value = 0.1f;
        float desire = 0;
        for (int i = 0; i < (int)values.size(); i++) {
            desire += m_logic->m_logicFunctions[m_needs.size() + 1][i]->getMaxMembership() * values[i];
            value += values[i];
        }
        desire /= value;
        return desire;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    std::vector<std::pair<std::string, float>> BaseAgent::getAppeals() {
        std::vector <std::pair<std::string, float>> output = std::vector<std::pair<std::string, float>>();
        for (int i = 0; i < (int)m_needs.size(); i++) {
            float desire = 0;
            float distToObject = INFINITY;
            SceneObject* nearest = nullptr;

            nearest = m_gameManager->getNearestObject(m_needs[i]->m_name, this);
            distToObject = glm::distance(getPosition(), nearest->getPosition());

            // Determine need
            std::vector<float> needs = std::vector<float>();
            for (int j = 0; j < (int)m_logic->m_logicFunctions[i].size(); j++) {
                needs.push_back(m_logic->m_logicFunctions[i][j]->getMembership(m_needs[i]->m_value));
            }

            // Determine distance
            std::vector<float> distances = std::vector<float>();
            for (int j = 0; j < (int)m_logic->m_logicFunctions[m_needs.size()].size(); j++) {
                distances.push_back(m_logic->m_logicFunctions[m_needs.size()][j]->getMembership(distToObject));
            }

            // Determine desirability
            std::vector<std::vector<float>> values = std::vector<std::vector<float>>();
            values.push_back(needs);
            values.push_back(distances);

            std::vector<float> appeals = std::vector<float>();
            for (int j = 0; j < (int)m_logic->m_logicFunctions[m_needs.size() + 1].size(); j++) {
                std::pair<float, int> result = calcLogic(i, values, m_appealSets[i][j]);
                appeals.push_back(result.first);
            }

            output.push_back(std::pair<std::string, float>(m_needs[i]->m_name, defuzzify(appeals)));
        }
        return output;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    struct lessThanValue {
        inline bool operator()(const std::pair<std::string, float> a, const std::pair<std::string, float> b) {
            return a.second > b.second;
        }
    };

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::update(float deltaTime) {
        SceneObject::update(deltaTime);

        // Decay needs
        for (int i = 0; i < (int)m_needs.size(); i++) {
            if (m_needs[i]->m_value > 0) {
                m_needs[i]->m_value -= m_needs[i]->m_decay * deltaTime;
            }
            else {
                m_health -= 0.01f;
            }
        }

        // Determine how appealing needs are
        std::vector<std::pair<std::string, float>> appeals = getAppeals();
        std::sort(appeals.begin(), appeals.end(), lessThanValue());

        SceneObject* target = nullptr;
        // Most appealing of needed type
        target = m_gameManager->getNearestObject(appeals[0].first, this);

        moveTowards(target->getPosition(), deltaTime);

        for (int i = 0; i < (int)m_needs.size(); i++) {
            m_gameManager->getNearestObject(m_needs[i]->m_name, this);

            if (glm::distance(m_position, target->getPosition()) <= target->getRadius()) {
                BaseResource* resource = dynamic_cast<BaseResource*>(target);
                BaseAgent* agent = dynamic_cast<BaseAgent*>(target);
                if (resource != nullptr) {
                    if (getNeedValue(resource->getType()) + resource->getHarvestRate() * deltaTime <= 1.0f) {
                        addValueToNeed(resource->getType(), resource->consume(deltaTime));
                    }
                }
                else if (agent != nullptr) {
                    if (getNeedValue(agent->getType()) + m_damage * deltaTime <= 1.0f) {
                        addValueToNeed(agent->getType(), m_damage);
                    }
                }
            }
        }
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    void BaseAgent::draw(aie::Renderer2D* renderer) {
        SceneObject::draw(renderer);
    }
}