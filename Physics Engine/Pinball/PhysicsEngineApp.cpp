#include "PhysicsEngineApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "SliderJoint.h"
#include "SpringJoint.h"
#include "SoftSphere.h"
#include "SoftBox.h"

#include <Texture.h>
#include <Font.h>
#include <Input.h>

#include <iostream>

#include <Windows.h>
#include <mmsystem.h>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

// Game Sounds from: https://sfbgames.com

/****************************************************************************************************
****************************************************************************************************/
char* getExeFolderPath() {
	// Obtain the path to the folder that contains the executable.
	wchar_t buffer[1000];
	GetModuleFileName(GetModuleHandle(NULL), buffer, sizeof(buffer) - 1);
	
	char ch[1000];
	char defChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, ch, 1000, &defChar, NULL);

	for (int i = 999; i >= 0; i--) {
		if (ch[i] == '/') {
			// cut off everything after
			ch[i+1] = 0;
			break;
		}
		else if (ch[i] == '\\') {
			// cut off everything after
			ch[i+1] = 0;
			break;
		}
	}
	
	return ch;
}

/****************************************************************************************************
Causes a large amount of lag, doesn't path correctly in release version
****************************************************************************************************/
void PhysicsEngineApp::playSound(char* fileName) {
	/*
	char path[1000];
	strcpy(path, getExeFolderPath());
	strcat(path, fileName);
	//std::cout << path << std::endl;
	wchar_t wtext[1000];
	mbstowcs(wtext, path, strlen(path) + 1);
	LPWSTR ptr = wtext;

	PlaySound(NULL, 0, 0);
	PlaySound(ptr, NULL, SND_FILENAME | SND_ASYNC);
	*/
}

/************************************************************************************************************
*************************************************************************************************************/
PhysicsEngineApp::PhysicsEngineApp() {
}

/************************************************************************************************************
*************************************************************************************************************/
PhysicsEngineApp::~PhysicsEngineApp() {
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsEngineApp::startup() {
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	srand((unsigned)time(NULL));

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.0005f);

	// Initialising variables for scene creation
	float x = 0;
	float y = 0;
	float wallStrength = 0;
	float bumperStrength = 0;

	Plane* plane;
	Sphere* sphere;
	Sphere* sphere2;
	Box* box;
	Bumper* bumper;
	SoftSphere* softSphere;
	SoftBox* softBox;

	m_mode = PinBall;

	switch (m_mode) {
	// A bunch of different objects
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
	// A large amount of random objects placed in a box
	case BoxOfStuff:
		m_physicsScene->setGravity(glm::vec2(0, -10));
		m_physicsScene->setTimeStep(0.01f);

		// Bottom
		plane = new Plane(glm::vec2(0, 1), -200);
		plane->setDestroyer(true);
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
				m_physicsScene->addActor(new Box(glm::vec2(x, y), rot, mass, glm::vec2(mass, mass)));
			}
			else {
				m_physicsScene->addActor(new Sphere(glm::vec2(x, y), mass, mass));
			}
		}
		break;
	// A Demo which displays projectile motion
	case ContinuousDemo:
		m_physicsScene->setGravity(glm::vec2(0, -10));

		glm::vec2 startPos = glm::vec2(-590, -310);
		glm::vec2 startVel = glm::vec2(100, 75);

		setupContinuousDemo(startPos, startVel, -10);
		m_physicsScene->addActor(new Sphere(startPos, 1, 5, startVel));
		break;
	// A ball rolling around
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
	// The pinball simulation
	case PinBall:
		m_physicsScene->setGravity(glm::vec2(0, -50));

		wallStrength = 0.25f;
		bumperStrength = 3.0f;

		// Launch Chamber
		box = new Box(glm::vec2(215, -180), 0, 1, glm::vec2(5, 145), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		box = new Box(glm::vec2(237, 0), (float)M_PI*0.6f, 1, glm::vec2(60, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		box = new Box(glm::vec2(220, 185), 0, 1, glm::vec2(5, 137), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Ball
		sphere = new Sphere(glm::vec2(235, -300), 1, 10);
		m_ball = sphere;
		m_physicsScene->addActor(sphere);
		// Launch Bumper
		bumper = new Bumper(glm::vec2(235, -315), (float)M_PI*0.0f, glm::vec2(10, 5), 50);
		m_launcher = bumper;
		m_physicsScene->addBumper(bumper);

		// Inner Walls
		// Top Left
		box = new Box(glm::vec2(-133, 202), (float)M_PI*0.45f, 1, glm::vec2(75, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Top
		box = new Box(glm::vec2(-25, 275), (float)M_PI*0.01f, 1, glm::vec2(100, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Top Right
		box = new Box(glm::vec2(125, 225), (float)M_PI*0.75f, 1, glm::vec2(75, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);

		// Flipper Area
		// Left
		box = new Box(glm::vec2(-185, -175), (float)M_PI*-0.15f, 1, glm::vec2(80, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Right
		box = new Box(glm::vec2(145, -175), (float)M_PI*0.15f, 1, glm::vec2(80, 5), wallStrength);
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Left Flipper
		bumper = new Bumper(glm::vec2(-90, -227), (float)M_PI*-0.15f, glm::vec2(25, 5), 50);
		m_leftFlipper = bumper;
		m_physicsScene->addBumper(bumper);
		// Right Flipper
		bumper = new Bumper(glm::vec2(50, -227), (float)M_PI*0.15f, glm::vec2(25, 5), 50);
		m_rightFlipper = bumper;
		m_physicsScene->addBumper(bumper);

		// Inner Bumpers
		// Top Left
		box = new Box(glm::vec2(-228, 295), (float)M_PI*1.25f, 1, glm::vec2(25, 5), glm::vec2(0, 0), bumperStrength, glm::vec4(0, 0, 1, 1));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Bottom Left
		box = new Box(glm::vec2(-228, -125), (float)M_PI*-1.25f, 1, glm::vec2(25, 5), glm::vec2(0, 0), bumperStrength, glm::vec4(0, 0, 1, 1));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Bottom Right
		box = new Box(glm::vec2(185, -125), (float)M_PI*1.25f, 1, glm::vec2(25, 5), glm::vec2(0, 0), bumperStrength, glm::vec4(0, 0, 1, 1));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Middle Left
		box = new Box(glm::vec2(-200, 30), (float)M_PI*1.25f, 1, glm::vec2(15, 15), glm::vec2(0, 0), bumperStrength, glm::vec4(0, 0, 1, 1));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);

		// Inner Bounce Balls
		// Top Center Left
		sphere = new Sphere(glm::vec2(-80, 175), 1, 15, glm::vec2(0, 0), bumperStrength, glm::vec4(1, 0, 0, 1));
		sphere->setIsKinematic(true);
		m_physicsScene->addActor(sphere);
		// Top Center Center
		sphere = new Sphere(glm::vec2(-5, 200), 1, 15, glm::vec2(0, 0), bumperStrength, glm::vec4(1, 0, 0, 1));
		sphere->setIsKinematic(true);
		m_physicsScene->addActor(sphere);
		// Top Center Right
		sphere = new Sphere(glm::vec2(50, 175), 1, 15, glm::vec2(0, 0), bumperStrength, glm::vec4(1, 0, 0, 1));
		sphere->setIsKinematic(true);
		m_physicsScene->addActor(sphere);
		// Middle Left
		sphere = new Sphere(glm::vec2(-100, -50), 1, 15, glm::vec2(0, 0), bumperStrength, glm::vec4(1, 0, 0, 1));
		sphere->setIsKinematic(true);
		m_physicsScene->addActor(sphere);
		// Middle Right
		sphere = new Sphere(glm::vec2(80, 0), 1, 15, glm::vec2(0, 0), bumperStrength, glm::vec4(1, 0, 0, 1));
		sphere->setIsKinematic(true);
		m_physicsScene->addActor(sphere);

		// Destruction Zone
		box = new Box(glm::vec2(-20, -295), 0, 1, glm::vec2(225, 20));
		box->setIsKinematic(true);
		box->setDestroyer(true);
		m_physicsScene->addActor(box);

		// Outer Walls
		// Right
		box = new Box(glm::vec2(m_screenWidth*0.2f, 0), 0, 1, glm::vec2(5, m_screenHeight*0.45f + 5));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Left
		box = new Box(glm::vec2(-m_screenWidth * 0.2f, 0), 0, 1, glm::vec2(5, m_screenHeight*0.45f + 5));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Top
		box = new Box(glm::vec2(0, m_screenHeight*0.45f), 0, 1, glm::vec2(m_screenWidth*0.2f + 5, 5));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);
		// Bottom
		box = new Box(glm::vec2(0, -m_screenHeight * 0.45f), 0, 1, glm::vec2(m_screenWidth*0.2f + 5, 5));
		box->setIsKinematic(true);
		m_physicsScene->addActor(box);

		break;
	// Used for general testing
	case Testing:
		m_physicsScene->setGravity(glm::vec2(0, -20));

		softBox = new SoftBox(glm::vec2(-150, 0), 1, glm::vec2(15, 15), 1);
		m_physicsScene->addActor(softBox);
		softBox = new SoftBox(glm::vec2(-100, 0), 1, glm::vec2(15, 15), 25);
		m_physicsScene->addActor(softBox);
		box = new Box(glm::vec2(-50, 0));
		m_physicsScene->addActor(box);

		softSphere = new SoftSphere(glm::vec2(50, 0), 1, 15, 1);
		m_physicsScene->addActor(softSphere);
		softSphere = new SoftSphere(glm::vec2(100, 0), 1, 15, 25);
		m_physicsScene->addActor(softSphere);
		sphere = new Sphere(glm::vec2(150, 0));
		m_physicsScene->addActor(sphere);

		m_physicsScene->addActor(new Plane(glm::vec2(0, 1), -100));

		break;
	}

	return true;
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsEngineApp::shutdown() {
	delete m_font;
	delete m_2dRenderer;

	delete m_physicsScene;

	for each (Bumper* bumper in m_bumpers) {
		delete bumper;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsEngineApp::update(float deltaTime) {
	aie::Input* input = aie::Input::getInstance();

	// Pauses and Unpauses with F1
	if (input->wasKeyPressed(aie::INPUT_KEY_F1)) {
		m_paused = !m_paused;
	}

	if (m_paused == false) {
		deltaTime = min(deltaTime, 0.1f);

		if (m_mode == PinBall) {
			if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
				m_launcher->m_bumper->setIsKinematic(false);
			}
			else if (input->wasKeyReleased(aie::INPUT_KEY_SPACE)) {
				m_launcher->resetBumper();
			}
			if (input->wasKeyPressed(aie::INPUT_KEY_LEFT)) {
				m_leftFlipper->m_bumper->setIsKinematic(false);
			}
			else if (input->wasKeyReleased(aie::INPUT_KEY_LEFT)) {
				m_leftFlipper->resetBumper();
			}
			if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT)) {
				m_rightFlipper->m_bumper->setIsKinematic(false);
			}
			else if (input->wasKeyReleased(aie::INPUT_KEY_RIGHT)) {
				m_rightFlipper->resetBumper();
			}
		}

		// Physics
		m_physicsScene->update(deltaTime);
		if (m_mode == PinBall) {
			if (m_ball->getMarkForRemove() == true) {
				playSound("/sounds/lose.wav");
				if (rand() % 5 == 0) {
					m_ball = new SoftSphere(glm::vec2(235, -285), 1, 10, 25);
					m_physicsScene->addActor(m_ball);
				}
				else {
					m_ball = new Sphere(glm::vec2(235, -300), 1, 10);
					m_physicsScene->addActor(m_ball);
				}
			}
		}
		m_physicsScene->checkRemoves();
		aie::Gizmos::clear();
		m_physicsScene->updateGizmos();
	}
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsEngineApp::draw() {
	clearScreen();

	m_2dRenderer->begin();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_screenWidth/2.0f, m_screenWidth / 2.0f, -m_screenHeight/ 2.0f, m_screenHeight / 2.0f, -1.0f, 1.0f));

	m_2dRenderer->end();
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsEngineApp::setupContinuousDemo(glm::vec2 startPos, glm::vec2 startVel, float gravity) {
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