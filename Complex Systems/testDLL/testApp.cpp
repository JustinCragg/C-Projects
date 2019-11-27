#include "testApp.h"
#include "BaseAgent.h"
#include "BaseResource.h"

#include <Texture.h>
#include <Font.h>
#include <Input.h>

#include <Windows.h>
#include <iostream>

float g_timeScale = 1.0f;

/************************************************************************************************************
************************************************************************************************************/
testApp::testApp() {

}

/************************************************************************************************************
************************************************************************************************************/
testApp::~testApp() {

}

/************************************************************************************************************
************************************************************************************************************/
bool testApp::startup() {
    srand((unsigned)time(NULL));

    m_2dRenderer = new aie::Renderer2D();

    m_windowHeight = Application::getWindowHeight();
    m_windowWidth = Application::getWindowWidth();

    m_gameManager = new flai::GameManager();
    
    flai::BaseResource* resource = (flai::BaseResource*)m_gameManager->createObject("water");
    resource->init(glm::vec2(m_windowWidth*0.5f, m_windowHeight*0.5f));
    resource = (flai::BaseResource*)m_gameManager->createObject("food");
    resource->init(glm::vec2(m_windowWidth*0.4f, m_windowHeight*0.4f));
    resource = (flai::BaseResource*)m_gameManager->createObject("shelter");
    resource->init(glm::vec2(m_windowWidth*0.6f, m_windowHeight*0.6f));

    flai::BaseAgent* agent = (flai::BaseAgent*)m_gameManager->createObject("prey");
    agent->init(glm::vec2(m_windowWidth*0.5f, m_windowHeight*0.6f));
    agent = (flai::BaseAgent*)m_gameManager->createObject("predator");
    agent->init(glm::vec2(m_windowWidth*0.5f, m_windowHeight*0.4f));
    agent = (flai::BaseAgent*)m_gameManager->createObject("random");
    agent->init(glm::vec2(m_windowWidth*0.6f, m_windowHeight*0.5f));

    return true;
}

/************************************************************************************************************
************************************************************************************************************/
void testApp::shutdown() {

    delete m_2dRenderer;
}

/************************************************************************************************************
************************************************************************************************************/
void testApp::update(float deltaTime) {
    aie::Input* input = aie::Input::getInstance();

    // Pausing
    if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
        if (g_timeScale == 1) {
            g_timeScale = 0;
        }
        else {
            g_timeScale = 1;
        }
    }
    // Reload Scripts
    if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL) && input->wasKeyPressed(aie::INPUT_KEY_R)) {
        m_gameManager->reloadScripts();
    }
    // Exit
    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) {
        quit();
    }

    deltaTime *= g_timeScale;
    m_gameManager->update(deltaTime);

}

/************************************************************************************************************
************************************************************************************************************/
void testApp::draw() {

    // wipe the screen to the background colour
    clearScreen();

    // begin drawing sprites
    m_2dRenderer->begin();

    // draw your stuff here!
    m_gameManager->draw(m_2dRenderer);

    // done drawing sprites
    m_2dRenderer->end();
}