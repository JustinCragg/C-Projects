#pragma once
#include "SceneObject.h"
#include "Shader.h"
#include "OBJMesh.h"

// The different shaders which can be used
enum ShaderType { ADVANCED, BASIC_TEXTURED, FLOOR, MULTI_LIGHT, NORMAL_MAP, PHONG, POST, SIMPLE };

// A class for drawing meshes loaded from an .obj file
class MeshObject : public SceneObject {
public:
    // The standard constructor - calls init
	MeshObject(glm::vec3 localPos, glm::vec3 localScale, char* fileName, ShaderType shaderType, SceneObject* parent);
    // The standard destructor
    ~MeshObject();
    // Initialises the MeshObject
    void init(glm::vec3 localPos, glm::vec3 localScale, char* fileName, ShaderType shaderType, SceneObject* parent);

    // Struct used for a vertex
	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;

		glm::vec4 tangent;
	};

    // Handles the updating of Gizmos for this object's children
	virtual void updateGizmos(SceneObject* _camera);
    // Handles the drawing of meshes for this object's children
    // lightsource is an array of LightSource pointers
    // drawMirror is true for when the objects is drawn onto a seperate buffer, eg. post-processing
	virtual void drawMesh(SceneObject* _camera, SceneObject** lightSource, bool drawMirror = false);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Sets the mesh that the object uses - updates the bounding sphere for culling
    void setMesh(aie::OBJMesh* mesh);
    
    // Sets a specific texture for the object to use
	void setTexture(char* fileName);

    // Sets whether this object is a render target
	void setRenderTarget(bool mirror) { m_renderToTarget = mirror; }

    // Sets the roughness of this object's material
    // 0 - Smooth
	void setRoughness(float roughness) { m_roughness = roughness; }
    // Sets the reflectiveness of this object's material
	void setReflect(float reflect) { m_reflect = reflect; }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setters
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the mesh of the object
    aie::OBJMesh* getMesh() { return m_mesh; }

protected:
    // The mesh of the object
	aie::OBJMesh* m_mesh = nullptr;
    // The program for handling the object's shaders
	aie::ShaderProgram* m_shaderProgram;

    // The type of shaders this object uses
	ShaderType m_shaderType;

    // The texture this object uses - nullptr for default textures
	aie::Texture* m_texture = nullptr;

    // Is this object a render target
	bool m_renderToTarget = false;

    // The roughness of the object
     // 0 - Smooth
	float m_roughness;
    // The reflectiveness of the object
    // 1 - Reflective
	float m_reflect;
};