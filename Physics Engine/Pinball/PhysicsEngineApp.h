#pragma once
#include <Application.h>
#include <Renderer2D.h>

#include "PhysicsScene.h"
#include "Bumper.h"

// The application which contains the physics simulation
class PhysicsEngineApp : public aie::Application {
public:
	// The standard constructor
	PhysicsEngineApp();
	// The standard destructor
	virtual ~PhysicsEngineApp();

	// Stores the width and height of the screen
	virtual void init(int screenWidth, int screenHeight) { m_screenWidth = screenWidth; m_screenHeight = screenHeight; }

	// Called once when the application is run
	virtual bool startup();
	// Called once when the application is closed
	virtual void shutdown();

	// Called every frame, calls the fixedUpdate and makeGizmo for the PhysicsScene
	virtual void update(float deltaTime);
	// Called every frame, handles the drawing of the objects on the scene
	virtual void draw();

	// Sets up and draws a simulation of projectile motion
	void setupContinuousDemo(glm::vec2 startPos, glm::vec2 startVel, float gravity);

	// Asynchronously plays a sounds - The file type must be a .wav 
	// The fileName must be a relative path from the program's .exe
	static void playSound(char* fileName);

	// The width of the application's screen
	int m_screenWidth;
	// The height of the application's screen
	int m_screenHeight;
protected:
	// Determines whether the application's update function is run
	bool m_paused = false;

	// A reference to the renderer used to draw non-gizmo objects to the screen
	aie::Renderer2D* m_2dRenderer;
	// The font which is used to draw text to the screen
	aie::Font* m_font;

	// A reference to the pysicsScene
	PhysicsScene* m_physicsScene;
	// The launcher used in the pinball simulation
	Bumper* m_launcher;
	// The left 'flipper' used in the pinball simulation
	Bumper* m_leftFlipper;
	// The right 'flipper' used in the pinball simulation
	Bumper* m_rightFlipper;
	// The ball used in the pinball simulation
	PhysicsObject* m_ball;
	// A list of all the bumpers used in the pinball simulation
	std::vector<Bumper*> m_bumpers;

	// The different modes which can be run in the simulation
	enum Mode { Everything, BoxOfStuff, ContinuousDemo, RollingBalls, Testing, PinBall };
	// The mode the simulation is running
	Mode m_mode;
};