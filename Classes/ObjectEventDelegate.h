#ifndef __OBJECT_EVENT_DELEGATE_H__
#define __OBJECT_EVENT_DELEGATE_H__

class GameObject;

class ObjectEventDelegate {
	virtual void onObjectCellMoved(GameObject* target, int destinationX, int destinationY) {}
};

#endif //!__OBJECT_EVENT_DELEGATE_H__