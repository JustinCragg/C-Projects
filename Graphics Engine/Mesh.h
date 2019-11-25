#pragma once
#include <glm/vec4.hpp>
#include "MeshObject.h"

// Class for a mesh, with manually designated vertices
// Inherits from MeshObject
class Mesh : public MeshObject {
public:
    // Standard constructor
	Mesh(glm::vec3 localPos, glm::vec3 localScale, ShaderType shaderType, SceneObject* parent);
    // Standard destructor
	virtual ~Mesh();

    // Handles the initalisation of the mesh for manual determining of vertices
	void init(unsigned int vertexCount, Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
    // Handles the initialisation of the mesh, for automatically adjusting to cover the screen
	void initFullscreen();

    // Handles the drawing of the mesh to the screen
    // lightSource is an array of LightSource pointers
    // drawMirror is true for when the objects is drawn onto a seperate buffer, eg. post-processing
	virtual void drawMesh(SceneObject* _camera, SceneObject** lightSource, bool drawMirror = false);

protected:
    // The number of tris in the mesh
	unsigned int m_triCount;

	// Vertex array object
	unsigned int m_vao;
	// Vertex buffer object
	unsigned int m_vbo;
	// Index buffer object
	unsigned int m_ibo;
};