#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"

class Area2D : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::AREA2D)

	Area2D() : _localPosition(0, 0){}

	virtual bool containsPoint(Vector2D point) = 0;

	void setLocalPos(Vector2D pos) {
		_localPosition = pos;
	}

	Vector2D getLocalPos() {
		return _localPosition;
	}

protected:

	Vector2D _localPosition;
};