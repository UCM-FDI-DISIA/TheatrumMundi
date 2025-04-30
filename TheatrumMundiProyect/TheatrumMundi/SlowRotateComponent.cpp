#include "SlowRotateComponent.h"
#include "../Components/Transform.h"

SlowRotateComponent::SlowRotateComponent(Transform* tr)
{
	_myObjTransform = tr;
}

SlowRotateComponent::~SlowRotateComponent()
{
}

void SlowRotateComponent::update()
{
	if (inrotate) {
		_myObjTransform->setRot(_myObjTransform->getRot()+angletorotatate);
		if (_myObjTransform->getRot()-1 >goalangle) {
			inrotate = false;
		}
	}
}

void SlowRotateComponent::startRotate(int angle)
{
	inrotate = true;
	goalangle += angle;
}
