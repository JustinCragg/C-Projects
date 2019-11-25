#include "MeshObject.h"
#include "Camera.h"
#include "Application3D.h"
#include "LightSource.h"

#include <glm/ext.hpp>

/************************************************************************************************************
************************************************************************************************************/
MeshObject::MeshObject(glm::vec3 localPos, glm::vec3 localScale, char* fileName, ShaderType shaderType, SceneObject* parent) : SceneObject(localPos, glm::vec3(0), localScale, parent) {
    init(localPos, localScale, fileName, shaderType, parent);
}

/************************************************************************************************************
************************************************************************************************************/
void MeshObject::init(glm::vec3 localPos, glm::vec3 localScale, char* fileName, ShaderType shaderType, SceneObject* parent) {
    setLocalPos(localPos);
    setLocalScale(localScale);

    m_shaderProgram = new aie::ShaderProgram();
    m_shaderType = shaderType;
    switch (m_shaderType) {
    case ADVANCED:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/advanced.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/advanced.frag");
        break;
    case BASIC_TEXTURED:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/basicTextured.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/basicTextured.frag");
        break;
    case FLOOR:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/floor.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/floor.frag");
        break;
    case MULTI_LIGHT:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/multiLight.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/multiLight.frag");
        break;
    case NORMAL_MAP:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");
        break;
    case PHONG:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");
        break;
    case POST:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");
        break;
    case SIMPLE:
        m_shaderProgram->loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
        m_shaderProgram->loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");
        break;
    default:
        break;
    }
    // Links the shaders
    if (m_shaderProgram->link() == false) {
        printf("Shader Error: %s\n", m_shaderProgram->getLastError());
    }

    // If a file is specified attempt to load it
    if (fileName != "") {
        m_mesh = new aie::OBJMesh();
        if (m_mesh->load(fileName, true, true) == false) {
            printf("Mesh Load Error!\n");
            return;
        }

        // Update bounding sphere
        std::vector<glm::vec4> positions;
        for each (aie::OBJMesh::Vertex vertex in m_mesh->getVertices()) {
            positions.push_back(vertex.position);
        }
        m_boundingSphere = new BoundingSphere(positions);
    }
}

/************************************************************************************************************
************************************************************************************************************/
MeshObject::~MeshObject() {
    delete m_mesh;
    delete m_shaderProgram;
    delete m_texture;
}

/************************************************************************************************************
************************************************************************************************************/
void MeshObject::setTexture(char* fileName) {
	m_texture = new aie::Texture();
	if (m_texture->load(fileName) == false) {
		printf("Failed to load texture!\n");
	}
}

/************************************************************************************************************
************************************************************************************************************/
void MeshObject::updateGizmos(SceneObject* _camera) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		SceneObject::updateGizmos(_camera);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void MeshObject::drawMesh(SceneObject* _camera, SceneObject** lightSource, bool renderToTarget) {
	if (onScreen(_camera, m_boundingChildrenSphere) == true) {
		if (onScreen(_camera, m_boundingSphere) == true) {
            Camera* camera = dynamic_cast<Camera*>(_camera);
            LightSource* light = dynamic_cast<LightSource*>(lightSource[0]);

			if (camera != nullptr && light != nullptr) {
				if (m_renderToTarget == false || (m_renderToTarget == true && renderToTarget == false)) {
					// Bind shader
					m_shaderProgram->bind();

					// Bind Transform
					glm::mat4 pvm = camera->getProjectionView() * m_worldTransform;
					m_shaderProgram->bindUniform("ProjectionViewModel", pvm);

                    glm::vec3 diffuse[5];
                    glm::vec3 specular[5];
                    float intensity[5];
                    int type[5];
                    glm::vec3 pos[5];
                    glm::vec3 direc[5];

					switch (m_shaderType) {
					case ADVANCED:
                        // Shader for advanced lighting - uses Oren-Nayar and Cook-Torrance
                        // Used for objects which don't have a normal map

						// Binding for vert shader
						m_shaderProgram->bindUniform("ModelMatrix", m_worldTransform);
						m_shaderProgram->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_worldTransform)));

						// Binding for frag shader
						m_shaderProgram->bindUniform("Ia", Application3D::getAmbientLight());
						m_shaderProgram->bindUniform("Id", light->getDiffuse());
						m_shaderProgram->bindUniform("Is", light->getSpecular());
						m_shaderProgram->bindUniform("lightDirection", light->getDirection());
						m_shaderProgram->bindUniform("cameraPosition", camera->getWorldPos());

						m_shaderProgram->bindUniform("roughness", m_roughness);
						m_shaderProgram->bindUniform("reflection", m_reflect);
						break;
					case BASIC_TEXTURED:
                        // Applies a basic diffuse texture to the object

						// Binding for frag shader
						m_shaderProgram->bindUniform("diffuseTexture", 0);
						m_texture->bind(0);
						break;
					case FLOOR:
                        // Shader for advanced lighting - uses Oren-Nayar and Cook-Torrance
                        // Also utilises multiple light types and sources
                        // Used for objects which don't have a normal map

                        // Binding for vert shader
                        m_shaderProgram->bindUniform("ModelMatrix", m_worldTransform);

                        // Binding for frag shader
                        m_shaderProgram->bindUniform("Ia", Application3D::getAmbientLight());
                        for (int i = 0; i < 5; i++) {
                            LightSource* light = dynamic_cast<LightSource*>(lightSource[i]);
                            if (light != nullptr) {
                                diffuse[i] = light->getDiffuse();
                                specular[i] = light->getSpecular();
                                intensity[i] = light->getIntensity();
                                type[i] = light->getType();
                                pos[i] = light->getWorldPos();
                                direc[i] = light->getDirection();
                            }
                            else {
                                diffuse[i] = glm::vec3(0);
                                specular[i] = glm::vec3(0);
                                intensity[i] = 0.0f;
                                type[i] = -1;
                                pos[i] = glm::vec3(0);
                                direc[i] = glm::vec3(0);
                            }
                        }
                        m_shaderProgram->bindUniform("Id", 5, diffuse);
                        m_shaderProgram->bindUniform("Is", 5, specular);
                        m_shaderProgram->bindUniform("lightIntensity", 5, intensity);
                        m_shaderProgram->bindUniform("lightType", 5, type);
                        m_shaderProgram->bindUniform("lightPos", 5, pos);
                        m_shaderProgram->bindUniform("lightDirec", 5, direc);

                        m_shaderProgram->bindUniform("cameraPosition", camera->getWorldPos());

                        m_shaderProgram->bindUniform("roughness", m_roughness);
                        m_shaderProgram->bindUniform("reflection", m_reflect);
                        break;
						break;
                    case MULTI_LIGHT:
                        // Shader for advanced lighting - uses Oren-Nayar and Cook-Torrance
                        // Also utilises multiple light types and sources

                        // Binding for vert shader
                        m_shaderProgram->bindUniform("ModelMatrix", m_worldTransform);
                        m_shaderProgram->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_worldTransform)));

                        // Binding for frag shader
                        m_shaderProgram->bindUniform("Ia", Application3D::getAmbientLight());
                        for (int i = 0; i < 5; i++) {
                            LightSource* light = dynamic_cast<LightSource*>(lightSource[i]);
                            if (light != nullptr) {
                                diffuse[i] = light->getDiffuse();
                                specular[i] = light->getSpecular();
                                intensity[i] = light->getIntensity();
                                type[i] = light->getType();
                                pos[i] = light->getWorldPos();
                                direc[i] = light->getDirection();
                            }
                            else {
                                diffuse[i] = glm::vec3(0);
                                specular[i] = glm::vec3(0);
                                intensity[i] = 0.0f;
                                type[i] = -1;
                                pos[i] = glm::vec3(0);
                                direc[i] = glm::vec3(0);
                            }
                        }
                        m_shaderProgram->bindUniform("Id", 5, diffuse);
                        m_shaderProgram->bindUniform("Is", 5, specular);
                        m_shaderProgram->bindUniform("lightIntensity", 5, intensity);
                        m_shaderProgram->bindUniform("lightType", 5, type);
                        m_shaderProgram->bindUniform("lightPos", 5, pos);
                        m_shaderProgram->bindUniform("lightDirec", 5, direc);

                        m_shaderProgram->bindUniform("cameraPosition", camera->getWorldPos());

                        m_shaderProgram->bindUniform("roughness", m_roughness);
                        m_shaderProgram->bindUniform("reflection", m_reflect);
                        break;
					case NORMAL_MAP:
                        // Shader for advanced lighting - uses Oren-Nayar and Cook-Torrance
                        // Used for objects which don't have a normal map

						// Binding for vert shader
						m_shaderProgram->bindUniform("ModelMatrix", m_worldTransform);
						m_shaderProgram->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_worldTransform)));

						// Binding for frag shader
						m_shaderProgram->bindUniform("Ia", Application3D::getAmbientLight());
						m_shaderProgram->bindUniform("Id", light->getDiffuse());
						m_shaderProgram->bindUniform("Is", light->getSpecular());
						m_shaderProgram->bindUniform("lightDirection", light->getDirection());
						m_shaderProgram->bindUniform("cameraPosition", camera->getWorldPos());

						m_shaderProgram->bindUniform("roughness", m_roughness);
						m_shaderProgram->bindUniform("reflection", m_reflect);
						break;
					case PHONG:
                        // Basic shader for simple lighting

						// Binding for vert shader
						m_shaderProgram->bindUniform("ModelMatrix", m_worldTransform);
						m_shaderProgram->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_worldTransform)));

						// Binding for frag shader
						m_shaderProgram->bindUniform("Ia", Application3D::getAmbientLight());
						m_shaderProgram->bindUniform("Id", light->getDiffuse());
						m_shaderProgram->bindUniform("Is", light->getSpecular());
						m_shaderProgram->bindUniform("lightDirection", light->getDirection());
						m_shaderProgram->bindUniform("cameraPosition", camera->getWorldPos());
						break;
					case SIMPLE:
                        // Simple shader for applying a constant colour - pink
						break;
					default:
						break;
					}
				}

				// Draw mesh
				m_mesh->draw();
			}
		}
		SceneObject::drawMesh(_camera, lightSource);
	}
}

/************************************************************************************************************
************************************************************************************************************/
void MeshObject::setMesh(aie::OBJMesh * mesh) {
    m_mesh = mesh; std::vector<glm::vec4> positions;
    for each (aie::OBJMesh::Vertex vertex in m_mesh->getVertices()) {
        positions.push_back(vertex.position);
    }
    m_boundingSphere = new BoundingSphere(positions);
}