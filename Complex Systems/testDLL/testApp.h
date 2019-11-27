#pragma once

#include <Application.h>
#include <Renderer2D.h>
#include "BaseAgent.h"

class testApp : public aie::Application {
public:
    // Constructor
    testApp();
    // Destructor
    virtual ~testApp();

    // Called once when app begins
    virtual bool startup();
    // Called once when app closes
    virtual void shutdown();

    // Called every frame
    virtual void update(float deltaTime);
    // Called every frame
    virtual void draw();

protected:
    // The renderer used
    aie::Renderer2D* m_2dRenderer;
    // The game manager
    flai::GameManager* m_gameManager = nullptr;

    // The height of the window
    int m_windowHeight;
    // The width of the window
    int m_windowWidth;
};