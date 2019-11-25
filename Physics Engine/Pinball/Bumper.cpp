#include "Bumper.h"
#include "Box.h"

/************************************************************************************************************
************************************************************************************************************/
Bumper::Bumper(glm::vec2 fixedPoint, float rot, glm::vec2 bumperSize, float strength) {
	m_rot = rot;
	m_direc = glm::normalize(glm::vec2(sin(m_rot), -cos(m_rot)));

	m_fixedPoint = new Box(fixedPoint, 0, 1, glm::vec2(0.5f, 0.5f));
	m_fixedPoint->setIsKinematic(true);
	m_bumper = new Box(fixedPoint - m_direc*7.5f, m_rot, 10, bumperSize);
	m_bumper->setIsKinematic(true);
	m_spring = new SpringJoint(m_fixedPoint, m_bumper, 50, strength);
}

/************************************************************************************************************
************************************************************************************************************/
Bumper::~Bumper() {

}

/************************************************************************************************************
************************************************************************************************************/
void Bumper::resetBumper() {
	m_bumper->setPosition(m_fixedPoint->getPosition() - m_direc*7.5f);
	m_bumper->setRotation(m_rot);
	m_bumper->clearVelocities();
	m_bumper->setIsKinematic(true);
}