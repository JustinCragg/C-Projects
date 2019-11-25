#include "PhysicsEngineApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsEngineApp();

	app->init(1280, 720);
	// initialise and loop
	app->run("AIE", app->m_screenWidth, app->m_screenHeight, true);

	// deallocation
	delete app;

	return 0;
}