#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>

class ScrollComponent : public ecs::Component {
private:
	Vector2D _dir;
	float _timeScroll;
	float _initialTimeScroll; //Activates if this time > 0
	bool _isRight;
	bool _isScrolling = false;
public:

	__CMPID_DECL__(ecs::cmp::SCROLL_COMPONENT)

	ScrollComponent(Vector2D dir, float time);
	void Scroll();
	void update() override; //ATTENTION
	void setScrolling(bool value);
};