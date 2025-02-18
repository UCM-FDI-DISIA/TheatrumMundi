#include "ScrollComponent.h"
#include <iostream>

using namespace std;

ScrollComponent::ScrollComponent(Vector2D dir, float time) : ecs::Component() {
	_dir = dir;
	_timeScroll = 0;
	_initialTimeScroll = time;
	cout << _initialTimeScroll << endl;
	_isRight = false;
	_isScrolling = false;
}

void ScrollComponent::Scroll() {
	cout << "CLICKKKKK" << endl;
	_timeScroll = _initialTimeScroll;
	cout << _timeScroll << endl;
}

void ScrollComponent::update()
{
	if (_timeScroll > 0) {
		_timeScroll--;
		if (!_isRight) { //SI LOS ELEMENTOS ESTAN SITUADOS A LA IZQUIERDA
			cout << _timeScroll << endl;
			if (_timeScroll == 0) {
				_isRight = !_isRight;
				cout << "LLEGAMOS A LA DERECHA" << endl;
			}
		}
		else { //SI LOS ELEMENTOS ESTAN SITUADOS A LA DERECHA
			cout << _timeScroll << endl;
			if (_timeScroll == 0) {
				_isRight = !_isRight;
				cout << "LLEGAMOS A LA IZQUIERDA" << endl;
			}
		}
	}
}

void ScrollComponent::setScrolling(bool value) {
	_isScrolling = value;
	cout << "SCROLLING: " << _isScrolling << endl;
}