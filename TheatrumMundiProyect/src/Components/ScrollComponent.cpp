#include "ScrollComponent.h"
#include <iostream>
#include "Transform.h"

#include "../ecs/Manager.h"

using namespace std;

ScrollComponent::ScrollComponent(Vector2D dir, float time) : ecs::Component() {
	_dir = dir;
	_timeScroll = 0;
	_initialTimeScroll = time;
	cout << _initialTimeScroll << endl;
	_isRight = false;
	_isScrolling = false;
	_myTransform = nullptr;
}

void ScrollComponent::initComponent() {
	auto mngr = _ent->getMngr();
	_myTransform = mngr->getComponent<Transform>(_ent);
	assert(_myTransform != nullptr);
}

void ScrollComponent::Scroll() {
	cout << "CLICKKKKK" << endl;
	_timeScroll = _initialTimeScroll;
	cout << _timeScroll << endl;

	_myTransform->getVel().set(_dir); //APLICAMOS LA VELOCIDAD
}

void ScrollComponent::update()
{
	//auto& vel = _myTransform->getVel();

	if (_timeScroll > 0) {

		_timeScroll--;

		if (!_isRight) { //The elements are in the left

			if (_dir.getX() < 0) { 
				_dir.setX(_dir.getX() * -1);
				_myTransform->getVel().set(_dir);
			}

			cout << _timeScroll << endl;
			if (_timeScroll == 0) {
				_isRight = !_isRight; //put the elements to the right at the end
				cout << "LLEGAMOS A LA DERECHA" << endl;
				_myTransform->getVel().set(0, 0);
			}
		}
		else { //the elements are in the right

			if (_dir.getX() > 0) {
				_dir.setX(_dir.getX() * -1);
				_myTransform->getVel().set(_dir);
			}

			cout << _timeScroll << endl;
			if (_timeScroll == 0) {
				_isRight = !_isRight; //put the elements to the left at the end
				cout << "LLEGAMOS A LA IZQUIERDA" << endl;
				_myTransform->getVel().set(0, 0);
			}
		}
	}
}

//void ScrollComponent::setScrolling(bool value) {
//	_isScrolling = value;
//	cout << "SCROLLING: " << _isScrolling << endl;
//}

bool ScrollComponent::isScrolling() {
	return _myTransform->getVel().magnitude() > 0;
}