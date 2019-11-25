#include "Game.h"

#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "NavAgent.h"
#include "NavMap.h"
#include "StateMachine.h"

#include "SceneObject.h"
#include "Predator.h"
#include "Prey.h"
#include "Food.h"

#include <Renderer2D.h>
#include <Font.h>
#include <Input.h>

#include <Windows.h>
#include <iostream>
#include <string>

// Multiplies game speed by this amount
const int TIME_SCALE = 1;

// How densely packed the nodes are on the navMap - must be between 1 and 10
const int NAV_MAP_NODE_DENSITY = 10;
// The weighting of the hCost on the overall cost of nodes
const float NAV_MAP_H_WEIGHT = 1.35f; // 1.35f
// PI
const float PI = 3.14159265f;

// The number of preds created
const int NUM_PRED = 5; // 3
// The number of preys created
const int NUM_PREY = 10; // 12
// The number of foods created
const int NUM_FOOD = 100; // 50
// How densely packed the homes are for foods
const int FOOD_HOME_DENSITY = 1;

// The game time
float Game::m_gameTime;
// The deltaTime for the current frame
float Game::m_deltaTime;

/****************************************************************************************************
****************************************************************************************************/
std::string getExeFolderPath() {
    // Obtain the path to the folder that contains the executable.
    char buffer[1000];
    GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof(buffer) - 1);
    char* pFileName = strrchr(buffer, '/');
    if (pFileName) {
        *pFileName = 0;
    }
    else {
        pFileName = strrchr(buffer, '\\');
        if (pFileName) {
            *pFileName = 0;
        }
    }
    return buffer;
}

/****************************************************************************************************
****************************************************************************************************/
float Game::getGameTime() {
    return m_gameTime;
}

/****************************************************************************************************
****************************************************************************************************/
float Game::getDeltaTime() {
    return m_deltaTime;
}

/****************************************************************************************************
Draws a dot at the position of the creature's target, of the specified colour
****************************************************************************************************/
void drawTarget(aie::Renderer2D* renderer, Creature* creature, float r, float g, float b) {
    renderer->setRenderColour(r, g, b);
    renderer->drawCircle(creature->m_targetPos.x, creature->m_targetPos.y, 2.5f, 0);
    renderer->setRenderColour(1, 1, 1);
}

/****************************************************************************************************
Draws a semi-transparent circle around the creature to show the 'vision' range of the creature
****************************************************************************************************/
void drawSight(aie::Renderer2D* renderer, Creature* creature) {
    math::Vector3 pos = ((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2];
    
    renderer->setRenderColour(1, 1, 1, 0.1f);
    renderer->drawCircle(pos.x, pos.y, (float)SHORT_DISTANCE, 100);
    renderer->setRenderColour(1, 1, 1);
}

/****************************************************************************************************
****************************************************************************************************/
Game::Game(int width, int height) {
    m_screenWidth = width;
    m_screenHeight = height;
}

/****************************************************************************************************
****************************************************************************************************/
Game::~Game() {}

/****************************************************************************************************
****************************************************************************************************/
bool Game::startup() {
    m_exeFolder = getExeFolderPath();

    m_rand.setSeedNow();
    m_gameTime = 0;

    // Renderer
    m_renderer2D = new aie::Renderer2D();

    // World Root
    m_worldRoot = new SceneObject;
    m_worldRoot->m_components.push_back(new Transform{ m_worldRoot, nullptr });
    m_worldRoot->m_components.push_back(new NavMap{ m_worldRoot, m_screenWidth, m_screenHeight, NAV_MAP_NODE_DENSITY, NAV_MAP_H_WEIGHT });

    // Camera
    m_camera = new SceneObject;
    m_camera->addComponent(new Transform{ m_camera, (Transform*)(m_worldRoot->getComponent(comp::Transform)) });
    Transform* transform = (Transform*) (m_camera->getComponent(comp::Transform));
    transform->setPosition(-m_screenWidth / 2.0f, -m_screenHeight / 2.0f);
    m_renderer2D->setCameraPos(transform->getPosition().x, transform->getPosition().y);

    aiManager = new AIManager;

    float offset = (15*11) - FOOD_HOME_DENSITY;
    int length = 0;
    int height = 0;

    // Fills the screen with homes for the foods
    for (float y = (m_screenHeight / 2.0f) - offset; y > (-m_screenHeight / 2.0f) + offset; y -= offset) {
        height++;
        for (float x = (-m_screenWidth / 2.0f) + offset; x < (m_screenWidth / 2.0f) - offset; x += offset) {
            length++;
            m_homes.push_back(math::Vector3{ x, y, 1 });
        }
    }

    // Create preds
    int numPred = NUM_PRED;
    for (int i = 0; i < numPred; i++) {
        m_preds.push_back(new Predator{ m_exeFolder, (Transform*)m_worldRoot->getComponent(comp::Transform) });
        aiManager->m_allPred.push_back(m_preds.back());
        // Random position
        int x = m_rand.rand() % 1280;
        x -= 640;
        int y = m_rand.rand() % 720;
        y -= 360;

        ((Transform*)m_preds.back()->getComponent(comp::Transform))->setPosition((float)x, (float)y);
        ((Transform*)m_preds.back()->getComponent(comp::Transform))->setScale(0.5f, 0.5f);
    }
    // Create preys
    int numPrey = NUM_PREY;
    for (int i = 0; i < numPrey; i++) {
        m_preys.push_back(new Prey{ m_exeFolder, (Transform*)m_worldRoot->getComponent(comp::Transform), (NavMap*)m_worldRoot->getComponent(comp::NavMap) });
        aiManager->m_allPrey.push_back(m_preys.back());
        // Random position
        int x = m_rand.rand() % 1280;
        x -= 640;
        int y = m_rand.rand() % 720;
        y -= 360;

        ((Transform*)m_preys.back()->getComponent(comp::Transform))->setPosition((float)x, (float)y);
        ((Transform*)m_preys.back()->getComponent(comp::Transform))->setScale(0.5f, 0.5f);
    }
    // Create foods
    int numFood = NUM_FOOD;
    for (int i = 0; i < numFood; i++) {
        m_foods.push_back(new Food{ m_exeFolder, (Transform*)m_worldRoot->getComponent(comp::Transform) });
        aiManager->m_allFood.push_back(m_foods.back());
        // Random position
        int x = m_rand.rand() % 1280;
        x -= 640;
        int y = m_rand.rand() % 720;
        y -= 360;

        // Assign them to a home
        m_foods.back()->m_homePos = m_homes[i%length*height];
        ((Transform*)m_foods.back()->getComponent(comp::Transform))->setPosition((float)x, (float)y);
        ((Transform*)m_foods.back()->getComponent(comp::Transform))->setScale(0.1f, 0.1f);
    }

    // Update initial Global Transforms
    transform = (Transform*)m_worldRoot->getComponent(comp::Transform);
    transform->updateTransforms();

    return true;
}

/****************************************************************************************************
****************************************************************************************************/
void Game::shutdown() {
    delete m_renderer2D;

    delete m_worldRoot;
    delete m_camera;
}

/****************************************************************************************************
****************************************************************************************************/
void Game::update(float deltaTime) {
    deltaTime *= TIME_SCALE;
    m_deltaTime = deltaTime;
    m_gameTime += deltaTime;

    aie::Input* input = aie::Input::getInstance();
    Transform* transform;

    // AI
    aiManager->update(deltaTime);
    // Movement
    aiManager->moveAll(deltaTime);
    // Updating Values
    aiManager->checkDead(deltaTime);

    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) {
        quit();
    }

    // Update Transforms
    transform = (Transform*)m_worldRoot->getComponent(comp::Transform);
    transform->updateTransforms();
}

/****************************************************************************************************
****************************************************************************************************/
void Game::draw() {
    // Clears the screen
    clearScreen();

    // ***** Enables the drawing of sprites to the screen
    m_renderer2D->begin();

    for (Creature* food : m_foods) {
        if (food->dead == false) {
            Sprite* sprite = (Sprite*)food->getComponent(comp::Sprite);
            // Draw food
            sprite->draw(m_renderer2D);

            //drawTarget(m_renderer2D, food, 0, 1, 0); // Uncomment to draw food targets
            //drawSight(m_renderer2D, food); // Uncomment to draw food vision
            //m_renderer2D->drawCircle(food->m_homePos.x, food->m_homePos.y, 2.5f); // Uncomment to draw food homes
        }
    }
    for (Creature* prey : m_preys) {
        if (prey->dead == false) {
            Sprite* sprite = (Sprite*)prey->getComponent(comp::Sprite);
            sprite->draw(m_renderer2D);

            //drawTarget(m_renderer2D, prey, 0, 0, 1); // Uncomment to draw prey targets
            //drawSight(m_renderer2D, prey); // Uncomment to draw food vision
            //m_renderer2D->drawCircle(prey->m_homePos.x, prey->m_homePos.y, 2.5f); // Uncomment to draw food homes
        }
    }
    for (Creature* pred : m_preds) {
        if (pred->dead == false) {
            Sprite* sprite = (Sprite*)pred->getComponent(comp::Sprite);
            sprite->draw(m_renderer2D);

            //drawTarget(m_renderer2D, pred, 1, 0, 0); // Uncomment to draw pred targets
            //drawSight(m_renderer2D, pred); // Uncomment to draw pred vision
        }
    }

    //((NavMap*)m_worldRoot->getComponent(comp::NavMap))->drawNodes(m_renderer2D); // Uncomment to draw paths taken by fleeing prey

    // ***** Disables the drawing of sprites to the screen
    m_renderer2D->end();
}