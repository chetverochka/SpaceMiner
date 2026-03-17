#include "GameObject.h"
#include "ObjectEventDelegate.h"

ObjectEventDelegate GameObject::ms_safeEventDelegate = ObjectEventDelegate();

USING_NS_CC;

GameObject::GameObject() :
	  m_eventDelegate(&ms_safeEventDelegate)
	, m_cellX(0)
	, m_cellY(0)
{

}

void GameObject::update(float deltaTime) {
	Sprite::update(deltaTime);

}

void GameObject::onEnter() {
	Sprite::onEnter();
	updatePosition();
}

void GameObject::onExit() {
	Sprite::onExit();
}

void GameObject::setCellX(int x) {
	if (m_cellX != x) {
		m_cellX = x;
		updatePosition();
	}
}

void GameObject::setCellY(int y) {
	if (m_cellY != y) {
		m_cellY = y;
		updatePosition();
	}
}

void GameObject::setCell(int x, int y) {
	if (m_cellX != x || m_cellY != y) {
		m_cellX = x;
		m_cellY = y;
		updatePosition();
	}
}

int GameObject::getCellX() const {
	return m_cellX;
}

int GameObject::getCellY() const {
	return m_cellY;
}

void GameObject::getCell(int* x, int* y) const {
	if (x)
		*x = m_cellX;
	if (y)
		*y = m_cellY;
}

void GameObject::updatePosition() {
	Vec2 gridStep(40.f, 40.f);

	Vec2 position;
	position.x = gridStep.x / 2 + m_cellX * gridStep.x;
	position.y = gridStep.y / 2 + m_cellY * gridStep.y;
	setPosition(position);

	if (m_eventDelegate) {
		m_eventDelegate->onObjectCellMoved(this, m_cellX, m_cellY);
	}
}

void GameObject::addEventListener(ObjectEventDelegate* listener) {
	m_eventDelegate = listener;
}

void GameObject::removeEventListener() {
	m_eventDelegate = NULL;
}