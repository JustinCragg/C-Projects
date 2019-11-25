#include "BreakoutApp.h"
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <Gizmos.h>
#include <glm\ext.hpp>
#include "Physics.h"

static const int screenWidth = 1280;
static const int screenHeight = 720;
static const float brickWidth = 70;
static const float brickHeight = 30;

BreakoutApp::BreakoutApp() {

}

BreakoutApp::~BreakoutApp() {

}

bool BreakoutApp::startup() {
	// Increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	srand((unsigned)time(NULL));

	// Creating the bricks
	int columns = 14;
	int rows = 7;
	glm::vec2 pos = glm::vec2(-screenWidth*0.5f, 50);

	for (int y = 0; y < rows; y++) {
		glm::vec4 colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);
		for (int x = 0; x < columns; x++) {
			Brick* brick = new Brick();
			float xPos = (-screenWidth / 2 + brickWidth*1.75f) + (brickWidth + brickWidth * 0.1f) * x;
			float yPos = (screenHeight / 2 - brickHeight*2) - (brickHeight + brickHeight * 0.1f) * y;
			brick->init(glm::vec2(xPos, yPos), glm::vec2(brickWidth/2.0f, brickHeight/2.0f), colour);
			m_bricks.push_back(brick);
		}
	}

	// Creating the paddle
	pos = glm::vec2(0, -screenHeight / 2 + brickHeight * 2);
	m_paddle = new Paddle();
	m_paddle->init(pos, glm::vec2(brickWidth, brickHeight / 2), glm::vec4(1, 0, 1, 1));

	// Creating the ball
	float radius = 15;
	pos.y += brickHeight / 2 + radius;
	Ball* ball = new Ball();
	ball->init(pos, radius, glm::vec4(1, 1, 0, 1));
	m_balls.push_back(ball);

	return true;
}

void BreakoutApp::shutdown() {
	for each (Brick* brick in m_bricks) {
		delete brick;
	}
	m_bricks.clear();
	for each (Ball* ball in m_balls) {
		delete ball;
	}
	m_balls.clear();
	delete m_paddle;

	delete m_font;
	delete m_2dRenderer;
}

void BreakoutApp::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	for each (Brick* brick in m_bricks) {
		brick->update();
	}
	for each (Ball* ball in m_balls) {
		ball->update(deltaTime);
	}
	m_paddle->update(input, deltaTime);

	for (int b = 0; b < m_balls.size(); b++) {
		Ball* ball = m_balls[b];
		if (ball->m_active == true) {
			boxBallCollision(m_paddle, ball);
			for each (Box* box in m_bricks) {
				if (box->m_active == true) {
					if (boxBallCollision(box, ball)) {
						box->m_active = false;
						score++;
						if (score == m_balls.size() * 5 + m_balls.size()) {
							Ball* newBall = new Ball();
							newBall->init(ball->m_pos, 15, glm::vec4(1, 1, 0, 1));
							newBall->m_direc.x = ball->m_direc.x *= -1;
							newBall->m_direc.y = ball->m_direc.y *= -1;
							m_balls.push_back(newBall);
						}
					}
				}
			}
		}
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) {
		quit();
	}
}

void BreakoutApp::draw() {
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}