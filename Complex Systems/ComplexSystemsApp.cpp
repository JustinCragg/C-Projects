#include "ComplexSystemsApp.h"
#include "Prey.h"
#include "Food.h"
#include "Water.h"
#include "Shelter.h"

#include <Texture.h>
#include <Font.h>
#include <Input.h>

#include <Windows.h>

float ComplexSystemsApp::m_deltaTime;
GameManager* ComplexSystemsApp::m_gameManager;

/****************************************************************************************************
****************************************************************************************************/
std::string getExeFolderPath() {
	// Obtain the path to the folder that contains the executable.
	wchar_t buffer[1000];
	GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof(buffer) - 1);

	char ch[1000];
	char defChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, ch, 1000, &defChar, NULL);

	for (int i = 999; i >= 0; i--) {
		if (ch[i] == '/') {
			// cut off everything after
			ch[i + 1] = 0;
			break;
		}
		else if (ch[i] == '\\') {
			// cut off everything after
			ch[i + 1] = 0;
			break;
		}
	}

	return std::string(ch);
}

/************************************************************************************************************
************************************************************************************************************/
ComplexSystemsApp::ComplexSystemsApp() {

}

/************************************************************************************************************
************************************************************************************************************/
ComplexSystemsApp::~ComplexSystemsApp() {

}

/************************************************************************************************************
************************************************************************************************************/
bool ComplexSystemsApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

    m_windowHeight = Application::getWindowHeight();
    m_windowWidth = Application::getWindowWidth();

    m_gameManager = new GameManager();

	// Preys
    Prey* prey = new Prey(glm::vec2(m_windowWidth*0.5f, m_windowHeight*0.5f), getExeFolderPath() + "scripts/preyAI.script");
    m_gameManager->addObject(prey);
	// Foods
	Food* food = new Food(glm::vec2(m_windowWidth*0.25f, m_windowHeight*0.75f));
	m_gameManager->addObject(food);
	// Waters
	Water* water = new Water(glm::vec2(m_windowWidth*0.75f, m_windowHeight*0.75f));
	m_gameManager->addObject(water);
	// Shelters
	Shelter* shelter = new Shelter(glm::vec2(m_windowWidth*0.5f, m_windowHeight*0.25f));
	m_gameManager->addObject(shelter);

	return true;
}

/************************************************************************************************************
************************************************************************************************************/
void ComplexSystemsApp::shutdown() {

	delete m_gameManager;
	delete m_2dRenderer;
}

/************************************************************************************************************
************************************************************************************************************/
void ComplexSystemsApp::update(float deltaTime) {
    m_deltaTime = deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

    m_gameManager->update();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/************************************************************************************************************
************************************************************************************************************/
void ComplexSystemsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
    m_gameManager->draw(m_2dRenderer);

	// done drawing sprites
	m_2dRenderer->end();
}