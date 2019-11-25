#pragma once

#include "Ship.h"
#include "Sprite.h"
#include "Asteroid.h"
#include "SceneObject.h"

#include <Application.h>
#include <Renderer2D.h>

#include <string>

class Game : public aie::Application {
public:
    // Constructor, is passed the screen height, width and the difficulty
    Game(int width, int height, int difficulty);
    // Default Destructor
    virtual ~Game();

    // Called automatically once when the game starts
    virtual bool startup();
    // Called automatically once when the game ends
    virtual void shutdown();

    // Called automatically every frame
    // Delta time is passed through automatically, it is the amount of time the last frame took to complete
    virtual void update(float deltaTime);
    // Called automatically at the end of every frame, this is when sprites are drawn to the screen
    virtual void draw();

    // The width of the screen, assigned in the game's constructor
    int m_screenWidth;
    // The height of the screen, assigned in the game's constructor
    int m_screenHeight;

protected:
    // The difficulty of the game
    int m_gameDiff = 0;
    // The number of asteroids which are used in the game
    int m_numAsteroids = 0;
    // The max number of lives the player can have
    int m_maxLives = 0;
    // The speed the player moves at
    int m_playerSpeed = 0;

    // Used for calculating the score, counts from the game's start
    core::Timer m_gameTimer;
    // Stores the game score
    unsigned long m_score = 0;

    // Object which handles the rendering of Sprites
    aie::Renderer2D* m_renderer2D;
    // Stores the font being used
    aie::Font* m_font;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scene Objects
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // An empty SceneObject, which acts as the root of the scene
    SceneObject* m_worldRoot = nullptr;
    // Stores the location of the camera
    SceneObject* m_camera = nullptr;

    // The player character
    Ship* m_player1 = nullptr;
    // The vector of asteroids in the game
    std::vector<Asteroid*> m_asteroids;
};