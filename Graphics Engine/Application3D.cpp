#include "Application3D.h"
#include "Planet.h"
#include "Box.h"
#include "FlyCamera.h"
#include "MeshObject.h"
#include "LightSource.h"

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

// Initalising the static variables for deltaTime
float Application3D::m_deltaTime;
// Initalising the static variables for ambientLight
glm::vec3 Application3D::m_ambientLight;
// Initalising the static variables for renderTarget
aie::RenderTarget* Application3D::m_renderTarget;

/************************************************************************************************************
************************************************************************************************************/
Application3D::Application3D() {

}

/************************************************************************************************************
************************************************************************************************************/
Application3D::~Application3D() {
    delete m_worldRoot;

    delete m_renderTarget;
}

/************************************************************************************************************
************************************************************************************************************/
bool Application3D::startup() {
	setBackgroundColour(0.25f, 0.25f, 0.25f, 1);

	// initialise gizmo primitive counts
	aie::Gizmos::create(10000, 10000, 10000, 10000);

    // World root
	m_worldRoot = new SceneObject();
    // Camera
	m_camera = new FlyCamera(glm::vec3(0, 5, -5), (float)getWindowWidth() / (float)getWindowHeight(), m_worldRoot);
    // Lights
	glm::vec4 sunlight = glm::vec4(0.945f, 0.855f, 0.643f, 1);
    m_ambientLight = glm::vec4(0.25f, 0.25f, 0.25f, 1);
    for (int i = 0; i < 5; i++) {
        m_lights[i] = nullptr;
    }
	m_lights[0] = new LightSource(glm::vec3(0, 5, -5), glm::vec3(0,0,1), glm::vec3(0,0,1), LightSource::DIRECTIONAL, m_worldRoot);
	m_lights[0]->rotateLocalRot(glm::vec3(glm::pi<float>()*0.25f, 0, 0));
    m_lights[1] = new LightSource(glm::vec3(2.5f, 1, 2.5f), glm::vec3(0,1,0), glm::vec3(0,1,0), LightSource::POINT, m_worldRoot);
    ((LightSource*)m_lights[1])->setIntensity(10.0f);
    m_lights[2] = new LightSource(glm::vec3(-2.5f, 2.5f, -5), glm::vec3(1,0,0), glm::vec3(1,0,0), LightSource::SPOT, m_worldRoot);
    m_lights[2]->rotateLocalRot(glm::vec3(glm::pi<float>()*0.25f, 0, 0));

    // ***** Meshes
    MeshObject* mesh = nullptr;
	// Floor
	mesh = new MeshObject(glm::vec3(0, 0, 0), glm::vec3(1), "./models/cobblestone/cobblestones.obj", FLOOR, m_worldRoot);
    aie::OBJMesh* tempMesh = mesh->getMesh();
	mesh->setRoughness(0.25f);
	mesh->setReflect(0.25f);
	mesh = new MeshObject(glm::vec3(7.65f, 0, 0), glm::vec3(1), "", FLOOR, m_worldRoot);
    mesh->setMesh(tempMesh);
	mesh->setRoughness(0.25f);
	mesh->setReflect(0.25f);
	mesh = new MeshObject(glm::vec3(0, 0, 7.65f), glm::vec3(1), "", FLOOR, m_worldRoot);
    mesh->setMesh(tempMesh);
	mesh->setRoughness(0.25f);
	mesh->setReflect(0.25f);
	mesh = new MeshObject(glm::vec3(7.65f, 0, 7.65f), glm::vec3(1), "", FLOOR, m_worldRoot);
    mesh->setMesh(tempMesh);
	mesh->setRoughness(0.25f);
	mesh->setReflect(0.25f);
    // Sniper
    mesh = new MeshObject(glm::vec3(7.5f, 2.5f, 5), glm::vec3(7.5f), "./models/Sniper/Carbon.obj", MULTI_LIGHT, m_worldRoot);
    mesh->setRoughness(0.25f);
    mesh->setReflect(0.0f);
    // Sponge
    mesh = new MeshObject(glm::vec3(-2.5f, 1, 0), glm::vec3(0.1f), "./models/Sponge/sponge.obj", MULTI_LIGHT, m_worldRoot);
    mesh->setRoughness(0.1f);
    mesh->setReflect(0.1f);
    // Spear
    mesh = new MeshObject(glm::vec3(2.5f, 1, 0), glm::vec3(1), "./models/soulspear/soulspear.obj", MULTI_LIGHT, m_worldRoot);
    mesh->setRoughness(1.0f);
    mesh->setReflect(1.0f);
    // Apple
    mesh = new MeshObject(glm::vec3(-1, 1, 0), glm::vec3(0.01f), "./models/apple/apple.obj", MULTI_LIGHT, m_worldRoot);
    mesh->setRoughness(1.0f);
    mesh->setReflect(1.0f);

    // Post-Processing Quad
    // Comment out these three lines to disable blur
	m_postScreen = new Mesh(glm::vec3(0,0,1), glm::vec3(1), ShaderType::POST, m_worldRoot);
    m_postScreen->initFullscreen();
    m_postScreen->setRenderTarget(true);

    // Render target
	m_renderTarget = new aie::RenderTarget();
	if (m_renderTarget->initialise(1, getWindowWidth(), getWindowHeight()) == false) {
		printf("Render Target Error!\n");
		return false;
	}

	return true;
}

/************************************************************************************************************
************************************************************************************************************/
void Application3D::shutdown() {
	aie::Gizmos::destroy();
}

/************************************************************************************************************
************************************************************************************************************/
void Application3D::update(float deltaTime) {
	m_deltaTime = deltaTime;
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

    m_lights[0]->rotateLocalRot(glm::vec3(0, 0, deltaTime));

	m_worldRoot->update();
	m_worldRoot->updateGizmos(m_camera);

	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/************************************************************************************************************
************************************************************************************************************/
void Application3D::draw() {
	m_renderTarget->bind();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_camera->setPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	// Draw objects for render target
	m_worldRoot->drawMesh(m_camera, m_lights, true);

	m_renderTarget->unbind();
	clearScreen();

	// Draw objects
	m_worldRoot->drawMesh(m_camera, m_lights);

	// draw 3D gizmos
	aie::Gizmos::draw(m_camera->getProjectionView());
	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	aie::Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}