#pragma once
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"

#include <Application.h>
#include <Renderer2D.h>

#include <vector>

class BreakoutApp : public aie::Application {
public:

	BreakoutApp();
	virtual ~BreakoutApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;

	std::vector<Brick*> m_bricks;
	std::vector<Ball*> m_balls;
	Paddle* m_paddle;

	int score = 0;
};