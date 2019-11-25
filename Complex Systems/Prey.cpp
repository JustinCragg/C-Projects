#include "Prey.h"

#include <fstream>
#include <iostream>
#include <algorithm>

/************************************************************************************************************
************************************************************************************************************/
Prey::Prey(glm::vec2 position, std::string scriptPath) : BaseAgent(position, GameManager::PREY, glm::vec4(1,0,0,1)) {
	init(scriptPath);
}

/************************************************************************************************************
************************************************************************************************************/
Prey::Prey(glm::vec2 position, std::string scriptPath, glm::vec4 colour, float radius) : BaseAgent(position, GameManager::PREY, colour, radius) {
	init(scriptPath);
}

/************************************************************************************************************
************************************************************************************************************/
Prey::~Prey() {

}

/************************************************************************************************************
************************************************************************************************************/
void Prey::init(std::string scriptPath) {
	loadScript(scriptPath);
}

/************************************************************************************************************
************************************************************************************************************/
void Prey::loadScript(std::string scriptPath) {
	std::ifstream scriptFile(scriptPath, std::ios::in);
	std::string line;
	while (scriptFile.good() == true) {
		if (std::getline(scriptFile, line)) {
			if (line.size() != 0) {
				// execute line
				std::vector<std::string> words = splitString(line);
				std::string command = words[0];
				auto iter = m_allCommands.find(command);
				if (iter != m_allCommands.end()) {
					iter->second(words);
				}
				else {
					std::cout << "Attempted to run invalid command '" << command << "'\n";
				}
			}
		}
	}
}

float Prey::calcLogic(int needIndex, std::vector<std::vector<float>> values, std::vector<std::string> logics) {
	if (logics[0] == "AND") {
		std::vector<std::string> temp = logics;
		temp.erase(temp.begin());
		float a = calcLogic(needIndex, values, temp);
		temp.erase(temp.begin());
		float b = calcLogic(needIndex, values, temp);
		return FuzzyLogic::AND(a, b);
	}
	else if (logics[0] == "OR") {
		std::vector<std::string> temp = logics;
		temp.erase(temp.begin());
		float a = calcLogic(needIndex, values, temp);
		temp.erase(temp.begin());
		float b = calcLogic(needIndex, values, temp);
		return FuzzyLogic::OR(a, b);
	}
	else if (logics[0] == "NOT") {
		std::vector<std::string> temp = logics;
		temp.erase(temp.begin());
		float a = calcLogic(needIndex, values, temp);
		return FuzzyLogic::NOT(a);
	}
	else {
		int index = -1;
		for (int i = 0; i < m_logic->m_logicFunctions[needIndex].size(); i++) {
			if (m_logic->m_logicFunctions[needIndex][i]->_id == logics[0]) {
				return values[0][i];
			}
		}
		for (int i = 0; i < m_logic->m_logicFunctions[m_needs.size()].size(); i++) {
			if (m_logic->m_logicFunctions[m_needs.size()][i]->_id == logics[0]) {
				return values[1][i];
			}
		}
	}
	return 0;
}

/************************************************************************************************************
************************************************************************************************************/
std::vector<std::pair<std::string, float>> Prey::getDesires() {
	std::vector < std::pair<std::string, float>> output = std::vector<std::pair<std::string, float>>();
	for (int i = 0; i < m_needs.size(); i++) {
		float desire = 0;
		float distToObject = INFINITY;
		SceneObject* nearest = nullptr;

		if (i == 0) {
			nearest = m_gameManager->getMostAppealingObject(GameManager::ObjectType::FOOD, this);
		}
		else if (i == 1) {
			nearest = m_gameManager->getMostAppealingObject(GameManager::ObjectType::WATER, this);
		}
		else if (i == 2) {
			nearest = m_gameManager->getMostAppealingObject(GameManager::ObjectType::SHELTER, this);
		}
		distToObject = nearest->m_distance;

		// Determine need

		std::vector<float> needs = std::vector<float>();
		for (int j = 0; j < m_logic->m_logicFunctions[i].size(); j++) {
			needs.push_back(m_logic->m_logicFunctions[i][j]->getMembership(m_needs[i]->m_value));
		}

		// Determine distance
		std::vector<float> distances = std::vector<float>();
		for (int j = 0; j < m_logic->m_logicFunctions[m_needs.size()].size(); j++) {
			distances.push_back(m_logic->m_logicFunctions[m_needs.size()][j]->getMembership(distToObject));
		}

		// Determine desirability
		std::vector<std::vector<float>> values = std::vector<std::vector<float>>();
		values.push_back(needs);
		values.push_back(distances);

		std::vector<float> appeals = std::vector<float>();
		for (int j = 0; j < m_logic->m_logicFunctions[m_needs.size() + 1].size(); j++) {
			appeals.push_back(calcLogic(i, values, m_appealSets[i][j]));
		}

		float value = 0.1f;
		for (int i = 0; i < appeals.size(); i++) {
			desire += m_logic->m_logicFunctions[m_needs.size() + 1][i]->getMaxMembership() * appeals[i];
			value += appeals[0];
		}
		desire /= value;
		output.push_back(std::pair<std::string, float>(m_needs[i]->m_name, desire));
	}
	return output;
}

struct lessThanValue{
	inline bool operator() (const std::pair<std::string, float> a, const std::pair<std::string, float> b) {
		return a.second > b.second;
	}
};

/************************************************************************************************************
************************************************************************************************************/
void Prey::update() {
    BaseAgent::update();

	std::vector<std::pair<std::string, float>> appeals = getDesires();
	std::sort(appeals.begin(), appeals.end(), lessThanValue());

	SceneObject* target = m_gameManager->getMostAppealingObject(appeals[0].first, this);
	moveTowards(target->getPosition());
}

/************************************************************************************************************
************************************************************************************************************/
void Prey::draw(aie::Renderer2D* renderer) {
    BaseAgent::draw(renderer);
}