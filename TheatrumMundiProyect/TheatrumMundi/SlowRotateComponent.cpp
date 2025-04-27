#include "SlowRotateComponent.h"
#include "../Components/Transform.h"

SlowRotateComponent::SlowRotateComponent(Transform* tr)
{
	_myObjTransform = tr;
	goalangle = _myObjTransform->getRot();
}

SlowRotateComponent::~SlowRotateComponent()
{
}

void SlowRotateComponent::update()
{
	if (inrotate) {
		if (_myObjTransform->getRot() > goalangle-1) {
			inrotate = false;
		}else 
		_myObjTransform->setRot(_myObjTransform->getRot()+angletorotatate);
		
	}
}

void SlowRotateComponent::startRotate(int angle)
{
	if (!inrotate) {
		inrotate = true;
		goalangle += angle;
	}
}
