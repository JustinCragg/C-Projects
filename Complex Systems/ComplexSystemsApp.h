#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "SceneObject.h"

class ComplexSystemsApp : public aie::Application {
public:

	ComplexSystemsApp();
	virtual ~ComplexSystemsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

    static float getDeltaTime() { return m_deltaTime; }
    static GameManager* getGameManager() { return m_gameManager; }

protected:
	aie::Renderer2D* m_2dRenderer;

    int m_windowHeight;
    int m_windowWidth;

    static float m_deltaTime;
    
    static GameManager* m_gameManager;
};