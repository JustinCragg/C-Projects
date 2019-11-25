#include "Box.h"
#include <Gizmos.h>

/************************************************************************************************************
************************************************************************************************************/
Box::Box(glm::vec2 pos) : RigidBody(BOX, pos, 1, 0, glm::vec2(0, 0), 0.99f) {
	m_halfExtents = glm::vec2(15,15);
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 1.0f / 12.0f * m_mass * m_halfExtents.x * 2 * m_halfExtents.y * 2;
}

/************************************************************************************************************
************************************************************************************************************/
Box::Box(glm::vec2 pos, float rotation) : RigidBody(BOX, pos, 1, rotation, glm::vec2(0, 0), 0.99f) {
	m_halfExtents = glm::vec2(15, 15);
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 1.0f / 12.0f * m_mass * m_halfExtents.x * 2 * m_halfExtents.y * 2;
}

/************************************************************************************************************
************************************************************************************************************/
Box::Box(glm::vec2 pos, float rotation, float mass, glm::vec2 halfExtents, float elasticity) : RigidBody(BOX, pos, mass, rotation, glm::vec2(0, 0), elasticity) {
	m_halfExtents = halfExtents;
	m_colour = glm::vec4((rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, (rand() % 50 + 50) / 100.0f, 1);

	m_moment = 1.0f / 12.0f * m_mass * m_halfExtents.x * 2 * m_halfExtents.y * 2;
}

/************************************************************************************************************
************************************************************************************************************/
Box::Box(glm::vec2 pos, float rotation, float mass, glm::vec2 halfExtents, glm::vec2 velocity, float elasticity, glm::vec4 colour) : RigidBody(BOX, pos, mass, rotation, velocity, elasticity) {
	m_halfExtents = halfExtents;
	m_colour = colour;

	m_moment = 1.0f / 12.0f * m_mass * m_halfExtents.x * 2 * m_halfExtents.y * 2;
}

/************************************************************************************************************
************************************************************************************************************/
Box::~Box() {
}

/************************************************************************************************************
************************************************************************************************************/
bool Box::checkBoxCorners(Box* box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal, glm::vec2& contactForce) {
	float boxW = box->getHalfExtents().x * 2;
	float boxH = box->getHalfExtents().y * 2;
	float penetration = 0;
	for (float x = -box->getHalfExtents().x; x < boxW; x += boxW) {
		for (float y = -box->getHalfExtents().y; y < boxH; y += boxH) {
			// Position in world space
			glm::vec2 p = box->m_pos + x * box->m_localX + y * box->m_localY;
			// Position in box's local space
			glm::vec2 p0(glm::dot(p - m_pos, m_localX),
				glm::dot(p - m_pos, m_localY));
			float w2 = m_halfExtents.x, h2 = m_halfExtents.y;
			if (p0.y <= h2 && p0.y >= -h2) {
				// Check Corner
				if (p0.x >= 0 && p0.x <= w2) {
					numContacts++;
					contact += m_pos + w2 * m_localX + p0.y * m_localY;
					edgeNormal = m_localX;
					penetration = w2 - p0.x;
				}
				// Check Corner
				if (p0.x <= 0 && p0.x >= -w2) {
					numContacts++;
					contact += m_pos - w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX;
					penetration = w2 + p0.x;
				}
			}
			if (p0.x <= w2 && p0.x >= -w2) {
				// Check Corner
				if (p0.y >= 0 && p0.y <= h2) {
					numContacts++;
					contact += m_pos + p0.x * m_localX + h2 * m_localY;
					float pen0 = h2 - p0.y;
					if (pen0 <= penetration || penetration == 0) {
						penetration = pen0;
						edgeNormal = m_localY;
					}
				}
				// Check Corner
				if (p0.y <= 0 && p0.y >= -h2) {
					numContacts++;
					contact += m_pos + p0.x * m_localX - h2 * m_localY;
					float pen0 = h2 + p0.y;
					if (pen0 <= penetration || penetration == 0) {
						penetration = pen0;
						edgeNormal = -m_localY;
					}
				}
			}
		}
	}
	contactForce = penetration * edgeNormal;
	return (penetration != 0);
}

/************************************************************************************************************
************************************************************************************************************/
void Box::fixedUpdate(glm::vec2 gravity, float timeStep) {
	RigidBody::fixedUpdate(gravity, timeStep);

	// Recalculates local axes
	float cs = cosf(m_rot);
	float sn = sinf(m_rot);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

/************************************************************************************************************
************************************************************************************************************/
void Box::makeGizmo() {
	glm::vec2 p1 = m_pos - m_localX * m_halfExtents.x - m_localY * m_halfExtents.y;
	glm::vec2 p2 = m_pos + m_localX * m_halfExtents.x - m_localY * m_halfExtents.y;
	glm::vec2 p3 = m_pos - m_localX * m_halfExtents.x + m_localY * m_halfExtents.y;
	glm::vec2 p4 = m_pos + m_localX * m_halfExtents.x + m_localY * m_halfExtents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}