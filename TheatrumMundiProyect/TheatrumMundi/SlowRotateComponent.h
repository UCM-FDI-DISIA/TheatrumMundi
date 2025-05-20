#pragma once
#include "Component.h"
#include <functional>
class Transform;
class SlowRotateComponent : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::SLOW_ROTATE_COMPONENT);
	SlowRotateComponent(Transform* tr);
	~SlowRotateComponent();
	inline void setEndRotEvent(std::function<void()> a) {
		endrot = a;
	};
	void update() override;
	void startRotate(int angle);
protected:
	int angletorotatate = 1;
	int goalangle = 0;
	std::function<void()> endrot;
	bool inrotate = false;
	Transform* _myObjTransform;

};

