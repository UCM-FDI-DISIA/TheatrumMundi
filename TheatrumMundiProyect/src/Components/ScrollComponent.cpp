#include "ScrollComponent.h"
#include <iostream>
#include "Transform.h"
#include "../ecs/Manager.h"
#include <vector>


using namespace std;

ScrollComponent::ScrollComponent(int velocity, float time, Inverse isInverse, int numPhases) : ecs::Component() {
	
	_eventConnectionsScroll.insert({ STARTSCROLLING, {} });
	_eventConnectionsScroll.insert({ ENDEDSCROLLING, {} });

	if (isInverse == Inverse::NORMAL) {
		finalPhase = numPhases;
		phase = 0;
		_inverse = NORMAL;
	}
	else { //Inverse::INVERSE
		startPhase = 0 - numPhases;
		phase = 0;
		_inverse = INVERSE;
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

	if (_direction == UP || _direction == DOWN) {
		_path = UPDOWN;
	}
	if (_direction == LEFT || _direction == RIGHT) {
		_path = LEFTRIGHT;
	}

	_dir = Vector2D(0, 0);

	switch (_direction) {
		case UP:
			if (!startPhaseCheck()) {
				std::cout << "FASE: " << phase << std::endl;
				std::cout << "FASE INICIAL: " << startPhase << std::endl;
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
		e->setVel(_dir); 
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

		if (_timeScroll == 0) {

			auto& callbacks = _eventConnectionsScroll.at(ScrollComponent::ENDEDSCROLLING);

			for (CALLBACK callback : callbacks)
				callback();

			endScrollCallback();
		}
	} 
	else 
	{
		for (Transform* e : _objectsTransform) {
			e->setVel(Vector2D(0,0)); //DETENEMOS TODOS LOS OBJETOS AL NO HABER ACTIVADO EL SCROLL
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
	if (finalPhase + startPhase > 0) return finalPhase + startPhase;
	else return (-1 * (finalPhase + startPhase));
}

//void ScrollComponent::restartPosition()
//{
//	_timeScroll == 0.1f;
//	phase = 0;
//	if (_path == UPDOWN) {
//		if (_inverse == INVERSE) {
//			_dir = Vector2D(0, (10 * _velocity * _initialTimeScroll * startPhase));
//		}
//		else {
//			_dir = Vector2D(0, (10 * _velocity * _initialTimeScroll * (-finalPhase)));
//		}
//	}
//	else {
//		if (_inverse == INVERSE) {
//			_dir = Vector2D((10 * _velocity * _initialTimeScroll) * startPhase, 0);
//		}
//		else {
//			_dir = Vector2D((10 * _velocity * _initialTimeScroll) * (-finalPhase), 0);
//		}
//		
//	}
//		
//}

void ScrollComponent::addPhase()
{
	if (_inverse == NORMAL) finalPhase++;
	else startPhase--;
}

