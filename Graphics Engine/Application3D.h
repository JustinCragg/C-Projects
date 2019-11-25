#pragma once

#include "Application.h"
#include "SceneObject.h"
#include "Camera.h"
#include "LightSource.h"
#include "RenderTarget.h"

#include "Mesh.h"

#include <Input.h>
#include <glm/mat4x4.hpp>

// Base class for the application
class Application3D : public aie::Application {
public:
    // Standard constructor
	Application3D();
    // Standard destructor
	virtual ~Application3D();

    // Called when the applications starts
    // Handles the creation of the scene
	virtual bool startup();
    // Called when the application begins to shutdown
	virtual void shutdown();

    // Called every frame
    // Handles user input and updating transforms and gizmos
	virtual void update(float deltaTime);
    // Called every frame
    // Handles the drawing of Gizmos and Meshes
	virtual void draw();

    // Static function which returns the deltaTime for this frame
	static float getDeltaTime() { return m_deltaTime; }

    // Static function which returns the ambient lighting for the scene
	static glm::vec3 getAmbientLight() { return m_ambientLight; }

    // Static function which return a pointer the the render target
	static aie::RenderTarget* getRenderTarget() { return m_renderTarget; }

protected:
    // A mesh used for post-processing
    Mesh* m_postScreen;

    // The world root of the scene
	SceneObject* m_worldRoot;
    // The camera for the scene
	Camera* m_camera;
    // An array of the lights in the scene
	SceneObject* m_lights[5];

    // A static vector3 which is the ambient light in the scene
	static glm::vec3 m_ambientLight;

    // A static pointer to the render target
	static aie::RenderTarget* m_renderTarget;

    // A static float for the deltaTime
	static float m_deltaTime;
};