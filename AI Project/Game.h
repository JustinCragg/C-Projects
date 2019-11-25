#pragma once
#include "SceneObject.h"
#include "NavMap.h"
#include "AIManager.h"
#include "Creature.h"

#include <coreXorShift.h>

#include <Application.h>
#include <Renderer2D.h>
#include <Font.h>

// A class which handles the starting, exiting, updating and drawing for the game
class Game : public aie::Application {
public:
    // Constructor, is passed the screen height and width
    Game(int width, int height);
    // Default destructor
    ~Game();

    // Called automatically once when the game starts
    virtual bool startup();
    // Called automatically once when the game ends
    virtual void shutdown();

    // Called automatically every frame
    // Delta time is passed through automatically, it is the amount of time the last frame took to complete
    virtual void update(float deltaTime);
    // Called automatically at the end of every frame, this is when sprites are drawn to the screen
    virtual void draw();

    // Returns the time passed since the game begun - in game time
    static float getGameTime();
    // Returns the deltaTime for this frame
    static float getDeltaTime();

    // The width of the screen, assigned in the game's constructor
    int m_screenWidth;
    // The height of the screen, assigned in the game's constructor
    int m_screenHeight;
protected:
    // Absolute path to the exe
    std::string m_exeFolder;

    // Object which handles the rendering of Sprites
    aie::Renderer2D* m_renderer2D;

    // A random number generator
    core::XorShift32 m_rand;

    // The root object for the world - primarily used for Transforms
    SceneObject* m_worldRoot = nullptr;
    // The object for the camera - used to store its position
    SceneObject* m_camera = nullptr;

    // The manager which handles the decision making for the AI, the movement of the creatures and determing the creature's value and if they died
    AIManager* aiManager = nullptr;

    // All of the preds(red) to be used in the game
    std::vector<Creature*> m_preds;
    // All of the preys(blue) to be used in the game
    std::vector<Creature*> m_preys;
    // All of the foods(green) to be used in the game
    std::vector<Creature*> m_foods;

    // A vector of arbitrary points for the food to use as homes
    std::vector<math::Vector3> m_homes;

private:
    // The game time
    static float m_gameTime;
    // The deltaTime for the current frame
    static float m_deltaTime;
};