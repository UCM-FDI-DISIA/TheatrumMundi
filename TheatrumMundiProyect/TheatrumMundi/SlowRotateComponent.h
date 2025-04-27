#pragma once
#include "Component.h"
class Transform;
class SlowRotateComponent : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::SLOW_ROTATE_COMPONENT);
	SlowRotateComponent(Transform* tr);
	~SlowRotateComponent();
	void update() override;
	void startRotate(int angle);
protected:
	int angletorotatate = 1;
	int goalangle = 0;
	bool inrotate = false;
	Transform* _myObjTransform;

};

