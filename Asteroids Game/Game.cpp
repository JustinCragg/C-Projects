#include "Game.h"
#include "Physics.h"

#include <Vector2.h>
#include <Matrix2.h>

#include <Font.h>
#include <Input.h>
#include <Texture.h>

#include <Windows.h>
#include <iostream>

std::string g_exeFolder;

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
Game::Game(int width, int height, int difficulty) {
    m_screenWidth = width;
    m_screenHeight = height;

    m_gameDiff = difficulty + 1;
    m_numAsteroids = m_gameDiff * 10;
    m_maxLives = 22 - m_gameDiff * 2;
    m_playerSpeed = (m_gameDiff - 1) * 50;
}

/****************************************************************************************************
****************************************************************************************************/
Game::~Game() {}

/****************************************************************************************************
****************************************************************************************************/
bool Game::startup() {
    g_exeFolder = getExeFolderPath();

    m_gameTimer.start();

    // Renderer
    m_renderer2D = new aie::Renderer2D();
    m_font = new aie::Font((g_exeFolder + "/font/consolas.ttf").c_str(), 32);

    // World Root
    m_worldRoot = new SceneObject(nullptr);

    // Ship
    m_player1 = new Ship(m_worldRoot, g_exeFolder + "/textures/ship.png", 0, 0);

    m_player1->setPaths(g_exeFolder + "/textures/bullet.png", g_exeFolder + "/textures/ball.png");
    m_player1->setupCollider();
    // Turret
    for (int i = 0; i < m_maxLives; i++) {
        m_player1->addTurret(m_maxLives);
    }

    // Asteroids
    for (int i = 0; i < m_numAsteroids; i++) {
        m_asteroids.push_back(new Asteroid(m_worldRoot, g_exeFolder + "/textures/rock_large.png", 0, 0));
        m_asteroids[i]->setDisplayed(false);
        m_asteroids[i]->setupCollider();
    }

    // Camera
    m_camera = new SceneObject(m_worldRoot);
    float x = m_player1->getPosition().x - m_screenWidth / 2;
    float y = m_player1->getPosition().y - m_screenHeight / 4;
    m_camera->setPosition(x, y);

    return true;
}

/****************************************************************************************************
****************************************************************************************************/
void Game::shutdown() {
    delete m_renderer2D;
    delete m_font;

    for each(Asteroid* asteroid in m_asteroids) {
        delete asteroid;
    }
    delete m_player1;

    delete m_worldRoot;
    delete m_camera;
}

/****************************************************************************************************
****************************************************************************************************/
void Game::update(float deltaTime) {
    aie::Input* input = aie::Input::getInstance();

    // ***** Player *****
    // Movement
    m_player1->movePosition(0, m_playerSpeed * deltaTime);
    if (input->isKeyDown(aie::INPUT_KEY_UP) || input->isKeyDown(aie::INPUT_KEY_W)) {
        // Move player1 up
        m_player1->movePosition(0, 400 * deltaTime);
    }
    else if (input->isKeyDown(aie::INPUT_KEY_DOWN) || input->isKeyDown(aie::INPUT_KEY_S)) {
        // Move player1 down
        m_player1->movePosition(0, (-400-m_playerSpeed) * deltaTime);
    }
    if (input->isKeyDown(aie::INPUT_KEY_LEFT) || input->isKeyDown(aie::INPUT_KEY_A)) {
        // Move player1 left
        m_player1->movePosition(-400 * deltaTime, 0);
    }
    else if (input->isKeyDown(aie::INPUT_KEY_RIGHT) || input->isKeyDown(aie::INPUT_KEY_D)) {
        // Move player1 right
        m_player1->movePosition(400 * deltaTime, 0);
    }

    // Shooting
    if (input->isKeyDown(aie::INPUT_KEY_SPACE)) {
        m_player1->shoot();
    }

    // Bullets
    m_player1->bulletMove(deltaTime, m_playerSpeed);
    // Floating Ball / Turret / Lives
    m_player1->turretMove(deltaTime);

    // ***** Asteroids *****
    for (int i = 0; i < m_asteroids.size(); i++) {
        if (m_asteroids[i]->getDisplayed() == false) {
            // Create more, setting values randomly
            m_asteroids[i]->randSet(m_player1);
        }
        // Move
        m_asteroids[i]->autoMove(deltaTime, m_player1);
    }

    // Updates local and world transforms
    m_worldRoot->update();

    // ***** Collision *****
    // Asteroid
    for (int a = 0; a < m_asteroids.size(); a++) {
        Asteroid* asteroid = m_asteroids[a];
        // Bullet
        if (asteroid->getDisplayed() == true) {
            for each (Bullet* bullet in m_player1->getBulletPool()) {
                if (bullet->getDisplayed() == true) {
                    if (handleCollision(*asteroid, *bullet) == true) {
                        // On collision, destroys both objects
                        asteroid->setDisplayed(false);
                        bullet->setDisplayed(false);
                    }
                }
            }
        }
        // Asteroid
        if (asteroid->getDisplayed() == true) {
            for (int b = a + 1; b < m_asteroids.size(); b++) {
                Asteroid* other = m_asteroids[b];
                if (other->getDisplayed() == true) {
                    if (handleCollision(*asteroid, *other) == true) {
                        // On collision, bounces the asteroids off each other
                        bounce(*asteroid, *other, deltaTime);
                    }
                }
            }
        }
        // Ship
        if (asteroid->getDisplayed() == true) {
            if (handleCollision(*asteroid, *m_player1) == true) {
                // On collision, destroys the asteroid and removes a life from the player
                m_asteroids[a]->setDisplayed(false);
                if (m_player1->removeTurret() == false) {
                    // If the player runs out of lives the game ends
                    std::cout << "Your Score: " << m_score << std::endl;
                    quit();
                }
            }
        }
    }

    // Adds to the score (The number of asteroids in the game + The number of lives left * 0.1)
    m_score += (unsigned long)((m_numAsteroids + m_gameDiff*50) * deltaTime);

    // Updates local and world transforms
    m_worldRoot->update();

    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) {
        quit();
    }
}

/****************************************************************************************************
****************************************************************************************************/
void Game::draw() {
    // Clears the screen
    clearScreen();

    // Adjust the camera position to allow the player to move inside a small 'box' on the screen
    float x = clamp(m_camera->getPosition().x + m_screenWidth / 2, m_player1->getPosition().x - 75, m_player1->getPosition().x + 75) - m_screenWidth / 2;
    float y = clamp(m_camera->getPosition().y + m_screenHeight / 4, m_player1->getPosition().y - 50, m_player1->getPosition().y + 50) - m_screenHeight / 4;
    m_camera->setPosition(x, y);

    m_renderer2D->setCameraPos(m_camera->getPosition().x, m_camera->getPosition().y);

    // ***** Enables the drawing of sprites to the screen
    m_renderer2D->begin();

    for (int i = 0; i < m_asteroids.size(); i++) {
        m_asteroids[i]->draw(m_renderer2D);
    }

    // Draws the player's sprite as well as all sprites connected to it - lives, bullets
    m_player1->draw(m_renderer2D);

    // Draws the score at the top left of the screen
    m_renderer2D->drawText(m_font, std::to_string(m_score).c_str(), x + 5, y + m_screenHeight - 25);

    // ***** Disables the drawing of sprites to the screen
    m_renderer2D->end();
}