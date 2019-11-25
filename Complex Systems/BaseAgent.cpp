#include "BaseAgent.h"

#include <sstream>
#include <iterator>
#include <iostream>

/************************************************************************************************************
************************************************************************************************************/
BaseAgent::BaseAgent(glm::vec2 position, GameManager::ObjectType type) : SceneObject(position, type) {
	init();
}

/************************************************************************************************************
************************************************************************************************************/
BaseAgent::BaseAgent(glm::vec2 position, GameManager::ObjectType type, glm::vec4 colour, float radius) : SceneObject(position, type, colour, radius) {
	init();
}

/************************************************************************************************************
************************************************************************************************************/
BaseAgent::~BaseAgent() {

}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::init() {
	m_logic = new FuzzyLogic();
	initScriptCommands();
}

/************************************************************************************************************
************************************************************************************************************/
std::vector<std::string> BaseAgent::splitString(std::string string) {
	std::istringstream iss(string);
	return std::vector<std::string>(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>());
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::initScriptCommands() {
	m_allCommands["SET_NEEDS"] = [this](const std::vector<std::string> line) { onSetNeeds(line); };
	m_allCommands["SET_DECAY"] = [this](const std::vector<std::string> line) { onSetDecay(line); };
	m_allCommands["SET_FUNCTIONS"] = [this](const std::vector<std::string> line) { onSetFunctions(line); };
	m_allCommands["SET_APPEAL"] = [this](const std::vector<std::string> line) { onSetAppeal(line); };
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::onSetNeeds(std::vector<std::string> line) {
	assert(line.size() > 1);
	for (int i = 1; i < line.size(); i++) {
		m_needs.push_back(new Need(line[i]));
		m_needLevels.push_back(std::vector<std::string>());
		m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
		m_appealSets.push_back(std::vector<std::vector<std::string>>());
	}
	m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
	m_logic->m_logicFunctions.push_back(std::vector<MembershipFunction*>());
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::onSetDecay(std::vector<std::string> line) {
	assert(line.size() == 3);
	int needIndex = -1;
	for (int i = 0; i < m_needs.size(); i++) {
		if (m_needs[i]->m_name == line[1]) {
			needIndex = i;
			break;
		}
	}
	assert(needIndex != -1);
	m_needs[needIndex]->m_decay = (float)atof(line[2].c_str());
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::onSetFunctions(std::vector<std::string> line) {
	assert(line.size() >= 6);
	int index = -1;
	if (line[1] == "distance") {
		index = (int)m_needs.size();
	}
	else if (line[1] == "appeal") {
		index = (int)m_needs.size() + 1;
	}
	else {
		for (int i = 0; i < m_needs.size(); i++) {
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
void BaseAgent::onSetAppeal(std::vector<std::string> line) {
	assert(line.size() > 3);
	int needIndex = -1;
	for (int i = 0; i < m_needs.size(); i++) {
		if (m_needs[i]->m_name == line[1]) {
			needIndex = i;
			break;
		}
	}
	assert(needIndex != -1);
	int index = -1;
	for (int i = 0; i < m_logic->m_logicFunctions[m_needs.size() + 1].size(); i++) {
		if (m_logic->m_logicFunctions[m_needs.size() + 1][i]->_id == line[2]) {
			index = i;
			break;
		}
	}
	assert(index != -1);
	for (int i = 3; i < line.size(); i++) {
		m_appealSets[needIndex][index].push_back(line[i]);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::update() {
    SceneObject::update();

	for (int i = 0; i < m_needs.size(); i++) {
		m_needs[i]->m_value -= m_needs[i]->m_decay;
	}
}

/************************************************************************************************************
************************************************************************************************************/
void BaseAgent::draw(aie::Renderer2D* renderer) {
    SceneObject::draw(renderer);
}