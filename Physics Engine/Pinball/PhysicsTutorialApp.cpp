#include "PhysicsTutorialApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "SliderJoint.h"
#include "SpringJoint.h"

#include <Texture.h>
#include <Font.h>
#include <Input.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>


/************************************************************************************************************
*************************************************************************************************************/
PhysicsTutorialApp::PhysicsTutorialApp() {
}

/************************************************************************************************************
*************************************************************************************************************/
PhysicsTutorialApp::~PhysicsTutorialApp() {
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsTutorialApp::startup() {
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	srand((unsigned)time(NULL));

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.001f);

	float x = 0;
	float y = 0;

	Plane* plane;
	Sphere* sphere;
	Box* box;

	m_mode = Testing;

	switch (m_mode) {
	case Everything:
		m_physicsScene->setGravity(glm::vec2(0, -20));

		plane = new Plane(glm::vec2(-5, 1), -m_screenWidth * 0.06f);
		m_physicsScene->addActor(plane);
		plane = new Plane(glm::vec2(5, 1), -m_screenWidth * 0.06f);
		m_physicsScene->addActor(plane);
		sphere = new Sphere(glm::vec2(-100, m_screenHeight*0.4f));
		m_physicsScene->addActor(sphere);
		sphere = new Sphere(glm::vec2(100, m_screenHeight*0.5f));
		m_physicsScene->addActor(sphere);
		sphere = new Sphere(glm::vec2(0, m_screenHeight*0.3f));
		m_physicsScene->addActor(sphere);
		box = new Box(glm::vec2(-100, m_screenHeight*0.6f), (float)M_PI*0.1f);
		m_physicsScene->addActor(box);
		box = new Box(glm::vec2(0, m_screenHeight*0.65f), (float)M_PI*0.2f);
		m_physicsScene->addActor(box);
		box = new Box(glm::vec2(100, m_screenHeight*0.6f), (float)M_PI*0.0f);
		m_physicsScene->addActor(box);
		break;
	case BoxOfStuff:
		m_physicsScene->setGravity(glm::vec2(0, -10));
		m_physicsScene->setTimeStep(0.01f);

		// Bottom
		plane = new Plane(glm::vec2(0, 1), -200);
		m_physicsScene->addActor(plane);
		// Top
		plane = new Plane(glm::vec2(0, -1), -m_screenHeight * 0.5f);
		m_physicsScene->addActor(plane);
		// Left
		plane = new Plane(glm::vec2(1, 0), -m_screenWidth * 0.5f);
		m_physicsScene->addActor(plane);
		// Right
		plane = new Plane(glm::vec2(-1, 0), -m_screenWidth * 0.5f);
		m_physicsScene->addActor(plane);

		x = -m_screenWidth / 2.0f;
		y = 0;
		for (int i = 0; i < 50; i++) {
			float mass = (float)(rand() % 20 + 10);
			x += mass * 2 + 20;
			if (x > m_screenWidth / 2.0f - mass) {
				y -= 60;
				x -= m_screenWidth * 0.9f;
			}
			if (rand() % 2 == 0) {
				float rot = (float)(rand() % (int)(2 * M_PI * 100));
				m_physicsScene->addActor(new Box(glm::vec2(x, y), mass, glm::vec2(mass, mass), rot));
			}
			else {
				m_physicsScene->addActor(new Sphere(glm::vec2(x, y), mass, mass));
			}
		}
		break;
	case ContinuousDemo:
		m_physicsScene->setGravity(glm::vec2(0, -10));

		glm::vec2 startPos = glm::vec2(-590, -310);
		glm::vec2 startVel = glm::vec2(100, 75);

		setupContinuousDemo(startPos, startVel, -10);
		m_physicsScene->addActor(new Sphere(startPos, 1, 5, startVel));
		break;
	case RollingBalls:
		m_physicsScene->setGravity(glm::vec2(0, -30));

		plane = new Plane(glm::vec2(1, 0), -m_screenWidth * 0.5f + 10);
		m_physicsScene->addActor(plane);
		plane = new Plane(glm::vec2(1, 20), -m_screenHeight * 0.5f);
		m_physicsScene->addActor(plane);
		plane = new Plane(glm::vec2(0, 1), -m_screenHeight * 0.5f + 10);
		m_physicsScene->addActor(plane);
		plane = new Plane(glm::vec2(-1, 0), -m_screenWidth * 0.5f + 10);
		m_physicsScene->addActor(plane);

		sphere = new Sphere(glm::vec2(-m_screenWidth * 0.45f, -m_screenHeight * 0.25f), 1, 15, glm::vec2(0,0), 0.99f, glm::vec4(1,0,0,1));
		m_physicsScene->addActor(sphere);

		break;
	case Testing:
		m_physicsScene->setGravity(glm::vec2(0, -20));

		Sphere* ball1;
		Sphere* ball2;
		ball1 = new Sphere(glm::vec2(0, 0), 1, 15);
		ball1->setIsKinematic(true);
		m_physicsScene->addActor(ball1);
		for (int i = 1; i < 3; i++) {
			ball2 = new Sphere(glm::vec2(0 + i * 50, 0), 1, 15);
			m_physicsScene->addActor(ball2);
			//m_physicsScene->addActor(new SpringJoint(ball1, ball2, 50, 10, 0.1f, glm::vec2(0,-15), glm::vec2(0,15)));
			m_physicsScene->addActor(new SpringJoint(ball1, ball2, 50, 10, 0.1f));
			ball1 = ball2;
		}

		break;
	}

	return true;
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsTutorialApp::shutdown() {
	delete m_font;
	delete m_2dRenderer;

	delete m_physicsScene;
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsTutorialApp::update(float deltaTime) {
	deltaTime = glm::min(deltaTime, 0.1f);

	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsTutorialApp::draw() {
	clearScreen();

	m_2dRenderer->begin();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_screenWidth/2.0f, m_screenWidth / 2.0f, -m_screenHeight/ 2.0f, m_screenHeight / 2.0f, -1.0f, 1.0f));

	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	m_2dRenderer->end();
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsTutorialApp::setupContinuousDemo(glm::vec2 startPos, glm::vec2 startVel, float gravity) {
	float t = 0;
	float tStep = 0.2f;
	float radius = 5.0f;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	while (t <= 25) {
		glm::vec2 pos = startPos + (startVel*t + 0.5f*glm::vec2(0, gravity)*t*t);

		aie::Gizmos::add2DCircle(pos, radius, (int)radius+3, colour);
		t += tStep;
	}
}