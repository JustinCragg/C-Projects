#pragma once
#include <Application.h>
#include <Renderer2D.h>

#include "PhysicsScene.h"

class PhysicsTutorialApp : public aie::Application {
public:
	PhysicsTutorialApp();
	virtual ~PhysicsTutorialApp();

	virtual void init(int screenWidth, int screenHeight) { m_screenWidth = screenWidth; m_screenHeight = screenHeight; }

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void setupContinuousDemo(glm::vec2 startPos, glm::vec2 startVel, float gravity);

	int m_screenWidth;
	int m_screenHeight;
protected:
	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;

	PhysicsScene* m_physicsScene;

	enum Mode { Everything, BoxOfStuff, ContinuousDemo, RollingBalls, Testing };
	Mode m_mode;
};