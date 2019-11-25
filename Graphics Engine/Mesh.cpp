#include "Mesh.h"
#include "Camera.h"
#include "Application3D.h"
#include <gl_core_4_4.h>

/************************************************************************************************************
************************************************************************************************************/
Mesh::Mesh(glm::vec3 localPos, glm::vec3 localScale, ShaderType shaderType, SceneObject* parent) : MeshObject(localPos, localScale, "", shaderType, parent) {
    m_triCount = 0;

    m_vao = 0;
    m_vbo = 0;
    m_ibo = 0;
}

/************************************************************************************************************
************************************************************************************************************/
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

/************************************************************************************************************
************************************************************************************************************/
void Mesh::init(unsigned int vertexCount, MeshObject::Vertex* vertices, unsigned int indexCount, unsigned int* indices) {
	std::vector<glm::vec4> positions = std::vector<glm::vec4>();
	for (int i = 0; i < (int)vertexCount; i++) {
		positions.push_back(vertices[i].position);
	}
	m_boundingSphere = new BoundingSphere(positions);

	// Check the mesh is not already initialised
	assert(m_vao == 0);

	// Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind vertex array, the mesh wrapper
	glBindVertexArray(m_vao);

	// Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(MeshObject::Vertex), vertices, GL_STATIC_DRAW);

	// Enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshObject::Vertex), 0);

    // Enable third element
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshObject::Vertex), (void*)32);

	// Bind indicies if there are any
	if (indexCount != 0) {
		glGenBuffers(1, &m_ibo);

		// Bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// Fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		m_triCount = indexCount / 3;
	}
	else {
		m_triCount = vertexCount / 3;
	}

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/************************************************************************************************************
************************************************************************************************************/
void Mesh::initFullscreen() {
	assert(m_vao == 0);

    // Genearte buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

    // Bind vertex array, the mesh wrapper
	glBindVertexArray(m_vao);

    // Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // The screen space vertex positions
	float vertices[] = { -1,1,    -1,-1,    1,1,    -1,-1,    1,-1,    1,1 };

    // Fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Enable first element as position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_triCount = 2;
}

/************************************************************************************************************
************************************************************************************************************/
void Mesh::drawMesh(SceneObject* _camera, SceneObject** lightSource, bool renderToTarget) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		if (onScreen(_camera, m_boundingSphere) == true) {
			Camera* camera = dynamic_cast<Camera*>(_camera);

			if (camera != nullptr) {
				if (m_renderToTarget == false || (m_renderToTarget == true && renderToTarget == false)) {
					m_shaderProgram->bind();

                    switch (m_shaderType) {
                    case SIMPLE:
                        // Draw with a flat pink colour
                        break;
                    case ShaderType::BASIC_TEXTURED:
                        // Drawing a basic texture on the mesh
                        glm::mat4 pvm = camera->getProjectionView() * m_worldTransform;
                        m_shaderProgram->bindUniform("ProjectionViewModel", pvm);

                        m_shaderProgram->bindUniform("diffuseTexture", 0);

                        if (m_renderToTarget == true) {
                            // Use texture from buffer
                            Application3D::getRenderTarget()->getTarget(0).bind(0);
                        }
                        else {
                            // Use texture from file
                            m_texture->bind(0);
                        }
                        break;
                    case ShaderType::POST:
                        // Drawing a texture from buffer - used for post-processing
                        m_shaderProgram->bindUniform("colourTarget", 0);
                        Application3D::getRenderTarget()->getTarget(0).bind(0);
                        break;
                    default:
                        break;
                    }

                    // The actual drawing of the mesh
					if (renderToTarget == false) {
						glBindVertexArray(m_vao);

						if (m_ibo != 0) {
							glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
						}
						else {
							glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
						}
					}
				}
			}
		}
		SceneObject::drawMesh(_camera, lightSource);
	}
}