#pragma once
#include <glm/vec3.hpp>
#include <vector>

// Class for objects' bounding sphere
class BoundingSphere {
public:
    // Constructor for directly passing in a vector for vertices
	BoundingSphere(std::vector<glm::vec4> positions);
    // Constructor for passing in the centre and approximate radius of an object
	BoundingSphere(glm::vec3 centre, float radius);
    // Standard Constructor
	~BoundingSphere();

    // Returns the centre of the bounding sphere, relative to the object it is bounding
	glm::vec3 getLocalCentre() { return m_centre; }
    // Returns the radius of the bounding sphere
	float getRadius() { return m_radius; }

protected:
    // Fits a sphere around the vertex points
	void fit(std::vector<glm::vec4> positions);

    // The centre of the sphere
	glm::vec3 m_centre;
    // The radius of the sphere
	float m_radius;
};