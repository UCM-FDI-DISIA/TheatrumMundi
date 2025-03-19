#include "ScrollComponent.h"
#include <iostream>
#include "Transform.h"
#include "../ecs/Manager.h"
#include <vector>


using namespace std;

ScrollComponent::ScrollComponent(int velocity, float time, Inverse isInverse, int numPhases) : ecs::Component() {
	
	_eventConnectionsScroll.insert({ STARTSCROLLING, {} });
	_eventConnectionsScroll.insert({ ENDEDSCROLLING, {} });

	finalPhase = numPhases;
	if (isInverse == Inverse::NORMAL) {
		phase = startPhase;
	}
	else { //Inverse::INVERSE
		phase = finalPhase;
	}
	_timeScroll = 0;
	_initialTimeScroll = time;
	_velocity = velocity;
	//cout << _initialTimeScroll << endl;
	//_isScrolling = false;
}

ScrollComponent::~ScrollComponent()
{

}

void ScrollComponent::Scroll(Direction _direction) {

	cout << "Scroll Activated" << endl;

	_dir = Vector2D(0, 0);

	switch (_direction) {
		case UP:
			if (!startPhaseCheck()) {
				_dir = Vector2D(0, -1 * _velocity);
				phase--;
			}
			break;
		case DOWN:
			if (!finalPhaseCheck()) {
				_dir = Vector2D(0, _velocity);
				phase++;
			}
			break;
		case LEFT:
			if (!startPhaseCheck()) {
				_dir = Vector2D(-1 * _velocity, 0);
				phase--;
			}
			break;
		case RIGHT:
			if (!finalPhaseCheck()) {
				_dir = Vector2D(_velocity, 0);
				phase++;
			}
			break;
	}

	_timeScroll = _initialTimeScroll; //Initialize scrolling because time needs to be > 0

	std::cout << phase << endl;

	for (Transform* e : _objectsTransform) { //Apply direction to all objects
		e->getVel().set(_dir); 
	}

	auto& callbacks = _eventConnectionsScroll.at(ScrollComponent::STARTSCROLLING);

	for (CALLBACK callback : callbacks)
		callback();
}

void ScrollComponent::update()
{

	if (_timeScroll > 0) { //Scroll() activates this
		//cout << _timeScroll << endl;
		_timeScroll--;
		//if (!_isRight) { //The elements are in the left
		//
		//	//See if the direction is inverted to set it correctly and apply correct direction
		//	//HORIZONTALLY
		//	if (_dir.getX() < 0) {
		//		_dir.setX(_dir.getX() * -1);
		//		for (Transform* e : _objectsTransform) {
		//			e->getVel().set(_dir);
		//		}
		//	}
		//	//VERTICALLY
		//	if (_dir.getY() < 0) {
		//		_dir.setY(_dir.getY() * -1);
		//		for (Transform* e : _objectsTransform) {
		//			e->getVel().set(_dir);
		//		}
		//	}
		//
		//	//put the elements to the right at the end
		//	if (_timeScroll == 0) {
		//		cout << "RIGHT-DOWN" << endl;
		//		_isRight = !_isRight; 
		//
		//		for (Transform* e : _objectsTransform) { 
		//			e->getVel().set(0, 0);
		//		}
		//	}
		//}
		//else { //the elements are in the right
		//
		//	//See if the direction is inverted to set it correctly and apply correct direction
		//	//HORIZONTALLY
		//	if (_dir.getX() > 0) {
		//		_dir.setX(_dir.getX() * -1);
		//
		//		for (Transform* e : _objectsTransform) {
		//			e->getVel().set(_dir);
		//		}
		//	}
		//	//VERTICALLY
		//	if (_dir.getY() > 0) {
		//		_dir.setY(_dir.getY() * -1);
		//		for (Transform* e : _objectsTransform) {
		//			e->getVel().set(_dir);
		//		}
		//	}
		//
		//	//put the elements to the left at the end
		//	if (_timeScroll == 0) {
		//		cout << "LEFT-UP" << endl;
		//		_isRight = !_isRight; 
		//
		//		for (Transform* e : _objectsTransform) {
		//			e->getVel().set(0, 0);
		//		}
		//	}
		//}
		if (_timeScroll == 0) {
			auto& callbacks = _eventConnectionsScroll.at(ScrollComponent::ENDEDSCROLLING);

			for (CALLBACK callback : callbacks)
				callback();
		}
	} 
	else 
	{
		for (Transform* e : _objectsTransform) {
			e->getVel().set(Vector2D(0,0)); //DETENEMOS TODOS LOS OBJETOS AL NO HABER ACTIVADO EL SCROLL
		}
	}
}

bool ScrollComponent::isScrolling() {
	if (_timeScroll > 0) {
		return true;
	}
	return false;
}

bool ScrollComponent::connect(EVENT_TYPE eventType, CALLBACK action) {
	auto eventHashIt = _eventConnectionsScroll.find(eventType);
	if (eventHashIt == _eventConnectionsScroll.end()) return false;

	eventHashIt->second.push_back(action);

	return true;
}

void ScrollComponent::addElementToScroll(Transform* _objectT)
{
	assert(_objectT != nullptr);
	_objectsTransform.push_back(_objectT);
}

int ScrollComponent::numPhases()
{
	return finalPhase + startPhase;
}

