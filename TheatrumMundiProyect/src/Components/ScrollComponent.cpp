#include "ScrollComponent.h"
#include <iostream>
#include "Transform.h"
#include "../ecs/Manager.h"
#include <vector>

using namespace std;

ScrollComponent::ScrollComponent(Vector2D dir, float time) : ecs::Component() {
	_dir = dir;
	_timeScroll = 0;
	_initialTimeScroll = time;
	cout << _initialTimeScroll << endl;
	_isRight = false;
	_isScrolling = false;
	//_myTransform = nullptr;
}

ScrollComponent::~ScrollComponent()
{

}

//If we want to move the button itself use this
//void ScrollComponent::initComponent() {
//	auto mngr = _ent->getMngr();
//	_myTransform = mngr->getComponent<Transform>(_ent);
//	assert(_myTransform != nullptr);
//}

void ScrollComponent::Scroll() {

	cout << "Scroll Activated" << endl;

	_timeScroll = _initialTimeScroll; //Initialize scrolling because time needs to be > 0

	for (Transform* e : _objectsTransform) { //Apply direction to all objects
		e->getVel().set(_dir); 
	}
}

void ScrollComponent::update()
{

	if (_timeScroll > 0) { //Scroll() activates this

		//cout << _timeScroll << endl;
		_timeScroll--;

		if (!_isRight) { //The elements are in the left

			//See if the direction is inverted to set it correctly and apply correct direction
			//HORIZONTALLY
			if (_dir.getX() < 0) {
				_dir.setX(_dir.getX() * -1);
				for (Transform* e : _objectsTransform) {
					e->getVel().set(_dir);
				}
			}
			//VERTICALLY
			if (_dir.getY() < 0) {
				_dir.setY(_dir.getY() * -1);
				for (Transform* e : _objectsTransform) {
					e->getVel().set(_dir);
				}
			}

			//put the elements to the right at the end
			if (_timeScroll == 0) {
				cout << "RIGHT-DOWN" << endl;
				_isRight = !_isRight; 

				for (Transform* e : _objectsTransform) { 
					e->getVel().set(0, 0);
				}
			}
		}
		else { //the elements are in the right

			//See if the direction is inverted to set it correctly and apply correct direction
			//HORIZONTALLY
			if (_dir.getX() > 0) {
				_dir.setX(_dir.getX() * -1);

				for (Transform* e : _objectsTransform) {
					e->getVel().set(_dir);
				}
			}
			//VERTICALLY
			if (_dir.getY() > 0) {
				_dir.setY(_dir.getY() * -1);
				for (Transform* e : _objectsTransform) {
					e->getVel().set(_dir);
				}
			}

			//put the elements to the left at the end
			if (_timeScroll == 0) {
				cout << "LEFT-UP" << endl;
				_isRight = !_isRight; 

				for (Transform* e : _objectsTransform) {
					e->getVel().set(0, 0);
				}
			}
		}
	}
}

bool ScrollComponent::isScrolling() {
	if (_objectsTransform.empty()) return false;
	return _objectsTransform[0]->getVel().magnitude() > 0; //Check if the first element is moving
}

void ScrollComponent::addElementToScroll(Transform* _objectT)
{
	assert(_objectT != nullptr);
	_objectsTransform.push_back(_objectT);
}

