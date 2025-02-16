#include "ScrollComponent.h"
#include <iostream>

using namespace std;

ScrollComponent::ScrollComponent() : ecs::Component() {
	_dir = Vector2D(0, 0);
	_timeScroll = 0;
	_initialTimeScroll = 100.0f;
	_scrolling = false;
	_isRight = false;
}

void ScrollComponent::Scroll() {
	cout << "SCROLLING" << endl;
}

void ScrollComponent::update()
{
	if (_scrolling) {
		_timeScroll = _initialTimeScroll;
	}

	if (_timeScroll > 0) {
		_timeScroll--;
		cout << "SCROLLING" << endl;
	}
	else {
		_timeScroll == 0;
		_scrolling = false;
	}
}

bool ScrollComponent::isScrolling() {
	return _scrolling;
}