#include "GameObject.h"
#include "ObjectEventDelegate.h"

ObjectEventDelegate GameObject::ms_safeEventDelegate = ObjectEventDelegate();


GameObject::GameObject() :
	  m_eventDelegate(&ms_safeEventDelegate)
	, m_cellX(0)
	, m_cellY(0)
{

}