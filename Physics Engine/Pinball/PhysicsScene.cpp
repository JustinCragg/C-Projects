#include "PhysicsScene.h"
#include "PhysicsEngineApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Bumper.h"
#include "SoftSphere.h"
#include <iostream>
#include <algorithm>
#include <list>

/************************************************************************************************************
*************************************************************************************************************/
PhysicsScene::PhysicsScene() {
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0, 0);
}

/************************************************************************************************************
*************************************************************************************************************/
PhysicsScene::~PhysicsScene() {
	for each (PhysicsObject* actor in m_actors) {
		delete actor;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box,
};

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::checkRemoves() {
	std::vector<PhysicsObject*> listforDelete;

	for each (PhysicsObject* object in m_actors) {
		if (object->getMarkForRemove() == true) {
			listforDelete.push_back(object);
		}
	}

	for (int i = 0; i < listforDelete.size(); i++) {
		removeActor(listforDelete[i]);
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::checkForCollision() {
	int actorCount = (int)m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++) {
		for (int inner = outer + 1; inner < actorCount; inner++) {
			PhysicsObject* objectA = m_actors[outer];
			PhysicsObject* objectB = m_actors[inner];

			int shapeID1 = objectA->getShapeType();
			int shapeID2 = objectB->getShapeType();

			if (shapeID1 < -1 || shapeID2 < -1) {
				continue;
			}

			// Handles first object is softbody
			if (objectA->getShapeType() == SOFTBODY && objectB->getShapeType() != SOFTBODY) {
				for each (Sphere* sphere in ((SoftBody*)objectA)->getParticles()) {
					shapeID1 = sphere->getShapeType();

					int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
					fn collisionFunctionPtr = collisionFunctionArray[functionIDx];
					if (collisionFunctionPtr != nullptr) {
						if (collisionFunctionPtr(sphere, objectB) == true) {
							// For destruction
							if (sphere->getDestroyer() == true) {
								if (objectB->getDestroyer() == true) {
									continue;
								}
								else {
									objectB->setMarkForRemove(true);
								}
							}
							else {
								if (objectB->getDestroyer() == true) {
									objectA->setMarkForRemove(true);
								}
							}
							// Playing sound
							if (m_soundPlayed == false) {
								if ((objectA->getElasticity() + objectB->getElasticity()) / 2.0f > 1) {
									PhysicsEngineApp::playSound("/sounds/blip.wav");
									m_soundPlayed = true;
								}
								else {
									PhysicsEngineApp::playSound("/sounds/bump.wav");
									m_soundPlayed = true;
								}
							}
						}
					}
				}
			}
			// Handles second object is softbody
			else if (objectB->getShapeType() == SOFTBODY && objectA->getShapeType() != SOFTBODY) {
				for each (Sphere* sphere in ((SoftBody*)objectB)->getParticles()) {
					shapeID2 = sphere->getShapeType();

					int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
					fn collisionFunctionPtr = collisionFunctionArray[functionIDx];
					if (collisionFunctionPtr != nullptr) {
						if (collisionFunctionPtr(objectA, sphere) == true) {
							// Add additional functionality if neccesary
							// For destruction
							if (objectA->getDestroyer() == true) {
								if (objectB->getDestroyer() == true) {
									continue;
								}
								else {
									objectB->setMarkForRemove(true);
								}
							}
							else {
								if (sphere->getDestroyer() == true) {
									objectA->setMarkForRemove(true);
								}
							}
							// Playing sound
							if (m_soundPlayed == false) {
								if ((objectA->getElasticity() + objectB->getElasticity()) / 2.0f > 1) {
									PhysicsEngineApp::playSound("/sounds/blip.wav");
									m_soundPlayed = true;
								}
								else {
									PhysicsEngineApp::playSound("/sounds/bump.wav");
									m_soundPlayed = true;
								}
							}
						}
					}
				}
			}
			// Handles both objects being softbodies
			else if (objectA->getShapeType() == SOFTBODY && objectB->getShapeType() == SOFTBODY) {
				for each (Sphere* sphereA in ((SoftBody*)objectA)->getParticles()) {
					for each (Sphere* sphereB in ((SoftBody*)objectB)->getParticles()) {
						shapeID1 = sphereA->getShapeType();
						shapeID2 = sphereB->getShapeType();

						int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
						fn collisionFunctionPtr = collisionFunctionArray[functionIDx];
						if (collisionFunctionPtr != nullptr) {
							if (collisionFunctionPtr(sphereA, sphereB) == true) {
								// Add additional functionality if neccesary
								// For destruction
								if (objectA->getDestroyer() == true) {
									if (objectB->getDestroyer() == true) {
										continue;
									}
									else {
										objectB->setMarkForRemove(true);
									}
								}
								else {
									if (objectB->getDestroyer() == true) {
										objectA->setMarkForRemove(true);
									}
								}
								// Playing sound
								if (m_soundPlayed == false) {
									if ((objectA->getElasticity() + objectB->getElasticity()) / 2.0f > 1) {
										PhysicsEngineApp::playSound("/sounds/blip.wav");
										m_soundPlayed = true;
									}
									else {
										PhysicsEngineApp::playSound("/sounds/bump.wav");
										m_soundPlayed = true;
									}
								}
							}
						}
					}
				}
			}
			// Non softbody collisions
			else {
				int functionIDx = (shapeID1 * SHAPE_COUNT) + shapeID2;
				fn collisionFunctionPtr = collisionFunctionArray[functionIDx];
				if (collisionFunctionPtr != nullptr) {
					if (collisionFunctionPtr(objectA, objectB) == true) {
						// Add additional functionality if neccesary
						// For destruction
						if (objectA->getDestroyer() == true) {
							if (objectB->getDestroyer() == true) {
								continue;
							}
							else {
								objectB->setMarkForRemove(true);
							}
						}
						else {
							if (objectB->getDestroyer() == true) {
								objectA->setMarkForRemove(true);
							}
						}
						// Playing sound
						if (m_soundPlayed == false) {
							if ((objectA->getElasticity() + objectB->getElasticity()) / 2.0f > 1) {
								PhysicsEngineApp::playSound("/sounds/blip.wav");
								m_soundPlayed = true;
							}
							else {
								PhysicsEngineApp::playSound("/sounds/bump.wav");
								m_soundPlayed = true;
							}
						}
					}
				}
			}
		}
	}
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::plane2Plane(PhysicsObject* objA, PhysicsObject* objB) {
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::plane2Sphere(PhysicsObject* objA, PhysicsObject* objB) {
	return sphere2Plane(objB, objA);
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::plane2Box(PhysicsObject* objA, PhysicsObject* objB) {
	return box2Plane(objB, objA);
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::sphere2Plane(PhysicsObject* objA, PhysicsObject* objB) {
	Sphere* sphere = dynamic_cast<Sphere*>(objA);
	Plane* plane = dynamic_cast<Plane*>(objB);

	if (sphere != nullptr && plane != nullptr) {
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistFromOrigin();

		if (sphereToPlane < 0) {
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0) {
			sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));
			// COLLISION RESOLOUTION
			glm::vec2 contactPoint = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			plane->resolveCollision(sphere, contactPoint);
			return true;
		}

	}
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::sphere2Sphere(PhysicsObject* objA, PhysicsObject* objB) {
	Sphere* sphereA = dynamic_cast<Sphere*>(objA);
	Sphere* sphereB = dynamic_cast<Sphere*>(objB);

	if (sphereA != nullptr && sphereB != nullptr) {
		if (sphereA->getIsKinematic() == false || sphereB->getIsKinematic() == false) {
			glm::vec2 delta = sphereB->getPosition() - sphereA->getPosition();
			float distance = glm::length(delta);
			float intersection = sphereA->getRadius() + sphereB->getRadius() - distance;

			if (intersection > 0) {
				glm::vec2 contactForce = 0.5f * (distance - (sphereA->getRadius() + sphereB->getRadius()))*delta / distance;

				if (sphereA->getIsKinematic() == false && sphereB->getIsKinematic() == false) {
					sphereA->setPosition(sphereA->getPosition() + contactForce);
					sphereB->setPosition(sphereB->getPosition() - contactForce);
				}
				else if (sphereA->getIsKinematic() == false) {
					sphereA->setPosition(sphereA->getPosition() + contactForce * 2.0f);
				}
				else {
					sphereB->setPosition(sphereB->getPosition() - contactForce * 2.0f);
				}

				// COLLISION RESOLOUTION
				glm::vec2 contactPoint = 0.5f * (sphereA->getPosition() + sphereB->getPosition());
				sphereA->resolveCollision(sphereB, contactPoint);
				return true;
			}
		}
	}
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::sphere2Box(PhysicsObject* objA, PhysicsObject* objB) {
	return box2Sphere(objB, objA);
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::box2Plane(PhysicsObject* objA, PhysicsObject* objB) {
	Box* box = dynamic_cast<Box*>(objA);
	Plane* plane = dynamic_cast<Plane*>(objB);

	if (plane != nullptr && box != nullptr) {
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = std::fminf(box->getHalfExtents().x, box->getHalfExtents().y);
		float penetration = 0;

		// Determines which side of the plane the centre of mass is on
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistFromOrigin();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());

		for (float x = -box->getHalfExtents().x; x < box->getHalfExtents().x*2; x += box->getHalfExtents().x*2) {
			for (float y = -box->getHalfExtents().y; y < box->getHalfExtents().y*2; y += box->getHalfExtents().y*2) {

				// Position of corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				// Total velocity of the corner
				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getAngularVelocity() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				// If the corner is on the wrong side of the plane and moving further away, then their is a collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0) || (distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0)) {
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;

					if (comFromPlane >= 0) {
						if (penetration > distFromPlane) {
							penetration = distFromPlane;
						}
					}
					else {
						if (penetration < distFromPlane) {
							penetration = distFromPlane;
						}
					}
				}
			}
		}
		if (numContacts > 0) {
			// The average velocity going into the plane
			float collisionV = contactV / (float)numContacts;
			// The acceleration required to stop or reverse the velocity into the plane
			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);
			// The average position where the force will be applied
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();
			// The perpindicular distance the force is applied relative to
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));
			// The effective mass, describes how much the contact point velocity will change with the force being applied
			float mass0 = 1.0f / (1.0f / box->getMass() + (r*r) / box->getMoment());

			box->setPosition(box->getPosition() - plane->getNormal() * penetration);
			box->applyForce(acceleration * mass0, localContact);

			return true;
		}
	}
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::box2Sphere(PhysicsObject* objA, PhysicsObject* objB) {
	Box* box = dynamic_cast<Box*>(objA);
	Sphere* sphere = dynamic_cast<Sphere*>(objB);

	if (sphere != nullptr && box != nullptr) {
		if (box->getIsKinematic() == false || sphere->getIsKinematic() == false) {
			glm::vec2 circlePos = sphere->getPosition() - box->getPosition();
			float w2 = box->getHalfExtents().x;
			float h2 = box->getHalfExtents().y;

			int numContacts = 0;
			glm::vec2 contact(0, 0);

			// Checks corners
			for (float x = -w2; x <= w2; x += box->getHalfExtents().x * 2) {
				for (float y = -h2; y <= h2; y += box->getHalfExtents().y * 2) {
					glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
					glm::vec2 dp = p - circlePos;
					if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius()) {
						numContacts++;
						contact += glm::vec2(x, y);
					}
				}
			}

			glm::vec2* direction = nullptr;
			glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos));

			// Checks sides
			if (localPos.y < h2 && localPos.y > -h2) {
				if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius()) {
					numContacts++;
					contact += glm::vec2(w2, localPos.y);
					direction = new glm::vec2(box->getLocalX());
				}
				if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius())) {
					numContacts++;
					contact += glm::vec2(-w2, localPos.y);
					direction = new glm::vec2(-box->getLocalX());
				}
			}
			if (localPos.x < w2 && localPos.x > -w2) {
				if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius()) {
					numContacts++;
					contact += glm::vec2(localPos.x, h2);
					direction = new glm::vec2(box->getLocalY());
				}
				if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius())) {
					numContacts++;
					contact += glm::vec2(localPos.x, -h2);
					direction = new glm::vec2(-box->getLocalY());
				}
			}

			// Resolves collision
			if (numContacts > 0) {
				contact = box->getPosition() + (1.0f / numContacts) * (box->getLocalX()*contact.x + box->getLocalY()*contact.y);

				float pen = sphere->getRadius() - glm::length(contact - sphere->getPosition());
				glm::vec2 penVec = glm::normalize(contact - sphere->getPosition()) * pen;

				if (box->getIsKinematic() == false && sphere->getIsKinematic() == false) {
					box->setPosition(box->getPosition() + penVec * 0.5f);
					sphere->setPosition(sphere->getPosition() - penVec * 0.5f);
				}
				else if (box->getIsKinematic() == false) {
					box->setPosition(box->getPosition() + penVec);
				}
				else {
					sphere->setPosition(sphere->getPosition() - penVec);
				}

				box->resolveCollision(sphere, contact, direction);
				delete direction;
				return true;
			}
			else {
				delete direction;
			}
		}
	}
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
bool PhysicsScene::box2Box(PhysicsObject* objA, PhysicsObject* objB) {
	Box* boxA = dynamic_cast<Box*>(objA);
	Box* boxB = dynamic_cast<Box*>(objB);

	if (boxA != nullptr && boxB != nullptr) {
		if (boxA->getIsKinematic() == false || boxB->getIsKinematic() == false) {
			glm::vec2 boxPos = boxB->getPosition() - boxA->getPosition();

			glm::vec2 contact(0, 0);
			int numContacts = 0;
			float pen = 0;
			glm::vec2 norm(0, 0);
			glm::vec2 contactForce1, contactForce2;

			boxA->checkBoxCorners(boxB, contact, numContacts, pen, norm, contactForce1);

			if (boxB->checkBoxCorners(boxA, contact, numContacts, pen, norm, contactForce2)) {
				norm = -norm;
			}

			// Resolve colision
			if (numContacts > 0) {
				glm::vec2 contactForce = 0.5f * contactForce1 - contactForce2;

				if (boxA->getIsKinematic() == false && boxB->getIsKinematic() == false) {
					boxA->setPosition(boxA->getPosition() - contactForce);
					boxB->setPosition(boxB->getPosition() + contactForce);
				}
				else if (boxA->getIsKinematic() == false) {
					boxA->setPosition(boxA->getPosition() - contactForce * 2.0f);
				}
				else {
					boxB->setPosition(boxB->getPosition() + contactForce * 2.0f);
				}

				boxA->resolveCollision(boxB, contact / (float)numContacts, &norm);
				return true;
			}
		}
	}
	return false;
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::update(float deltaTime) {
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep) {
		for each (PhysicsObject* actor in m_actors) {
			actor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		checkForCollision();
	}
	m_soundCounter++;
	if (m_soundCounter >= 2) {
		m_soundPlayed = false;
		m_soundCounter -= 2;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::updateGizmos() {
	for each (PhysicsObject* actor in m_actors) {
		actor->makeGizmo();
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::debugScene() {
	int count = 0;
	for each (PhysicsObject* actor in m_actors) {
		std::cout << count << " : ";
		actor->debug();
		count++;
	}
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::addActor(PhysicsObject* actor) {
	m_actors.push_back(actor);
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::removeActor(PhysicsObject* actor) {
	m_actors.erase(std::remove(std::begin(m_actors), std::end(m_actors), actor), std::end(m_actors));
}

/************************************************************************************************************
*************************************************************************************************************/
void PhysicsScene::addBumper(Bumper* bumper) {
	addActor(bumper->m_fixedPoint);
	addActor(bumper->m_bumper);
	addActor(bumper->m_spring);
}
