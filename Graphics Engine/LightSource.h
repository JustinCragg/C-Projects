#pragma once
#include "SceneObject.h"

// Class used for light sources in the scene
class LightSource : public SceneObject {
public:
    // The different types of lights
    enum LightType { DIRECTIONAL, POINT, SPOT };

    // The standard constructor
	LightSource(glm::vec3 localPos, glm::vec3 diffuse, glm::vec3 specular, LightType type, SceneObject* parent);
    // The standard destructor
	~LightSource();

    // Handles the updating of this object's children
	virtual void update();
    // Handles the updating of the gizmos of this object's children
    // Adds a representation of this light as a Gizmo
	virtual void updateGizmos(SceneObject* _camera);
    // Handles the drawing of meshes for this object's children
    // lightsource is an array of LightSource pointers
	virtual void drawMesh(SceneObject* _camera, SceneObject** lightSource);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the direction the light is facing - only applicable for directional and spot lights
	glm::vec3 getDirection() { return m_direction; }
    // Returns the diffuse colour this light is emitting
	glm::vec3 getDiffuse() { return m_diffuse; }
    // Returns the specular colour this light is emitting
	glm::vec3 getSpecular() { return m_specular; }

    // Returns the intensity of the light - changes how much influence the light has on the colour of objects
    // Also determines the fall off for the point and spot lights
    float getIntensity() { return m_intensity; }

    // Returns the type of light this is - as an int
    int getType() { return m_lightType; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Sets the intensity of the light
    void setIntensity(float intensity) { m_intensity = intensity; }

protected:
    // The direction the light is pointing
	glm::vec3 m_direction;
    // The diffuse colour of the light
	glm::vec3 m_diffuse;
    // The specular colour of the light
	glm::vec3 m_specular;

    // The strength of the light
    float m_intensity = 1;

    // The type of light this is
    LightType m_lightType;
};