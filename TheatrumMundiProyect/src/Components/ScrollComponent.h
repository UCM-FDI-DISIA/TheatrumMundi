#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>

class ScrollComponent : public ecs::Component {
private:
	Vector2D _dir;
	float _timeScroll;
	float _initialTimeScroll;
	bool _scrolling;
	bool _isRight;
public:
	ScrollComponent();
	void setScrollingTrue() { _scrolling = true; }
	void setScrollingFalse() { _scrolling = false; }
	void Scroll();
	void update() override; //ATTENTION
	bool isScrolling();
};