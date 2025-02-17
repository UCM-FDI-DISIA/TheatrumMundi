#include "TriggerComponent.h"

#include "../sdlutils/InputHandler.h"
//#include "Area2D.h"
#include "RectArea2D.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include <iterator>
#include <algorithm>

TriggerComponent::TriggerComponent()
{
	_eventConnections.insert({ CURSOR_ENTERED, {} });
	_eventConnections.insert({ CURSOR_LEFT,    {} });
	_eventConnections.insert({ AREA_ENTERED,   {} });
	_eventConnections.insert({ AREA_LEFT,      {} });
}

void TriggerComponent::handleMouseInput()
{
	if (!ih().mouseMotionEvent()) return;
	
	RectArea2D* area = _ent->getMngr()->getComponent<RectArea2D>(_ent);
	if (area == nullptr) return; // Component needs Area2D to check the event
	
	bool mouseMovedInside = areaIsHovered(ih().getMousePos());
	
	if (mouseMovedInside && !_mouseIsInside)
	{
		_mouseIsInside = true;

		auto& callbacks = _eventConnections.at(TriggerComponent::CURSOR_ENTERED);

		for (CALLBACK callback : callbacks)
			callback();
	}
	else if (!mouseMovedInside && _mouseIsInside)
	{
		_mouseIsInside = false;

		auto& callbacks = _eventConnections.at(TriggerComponent::CURSOR_LEFT);

		for (CALLBACK callback : callbacks)
			callback();
	}
}

bool TriggerComponent::mouseIsIn()
{
	return _mouseIsInside;
}

bool TriggerComponent::areaIsHovered(std::pair<Sint32, Sint32> mousePos)
{
	RectArea2D* area = _ent->getMngr()->getComponent<RectArea2D>(_ent);
	if (area == nullptr) return false;

	return area->containsPoint(Vector2D(mousePos.first, mousePos.second));
}

void TriggerComponent::updateOverlappingEntities()
{
	_currentOverlappingEntities.clear();

	RectArea2D* area = _ent->getMngr()->getComponent<RectArea2D>(_ent);
	if (area == nullptr) return; // Can't check overlapping without Area2D

	const auto& allEntities = _ent->getMngr()->getEntities(); // Checking the entities with an Area2D that overlaps

	for (auto e : allEntities)
	{
		if (e != _ent) // Not self
		{
			RectArea2D* externArea = e->getMngr()->getComponent<RectArea2D>(e);
			if (externArea != nullptr && area->overlapsWithArea(externArea))
				_currentOverlappingEntities.push_back(e);
		}
	}
}

void TriggerComponent::updateTriggerState()
{
	RectArea2D* area = _ent->getMngr()->getComponent<RectArea2D>(_ent);
	if (area == nullptr) return;
	// Obtaining the exited entities and the unchanged ones
	std::list<ecs::entity_t> exitEnts = _currentOverlappingEntities;
	std::list<ecs::entity_t> unchangedEnts;
	
	auto entIt = exitEnts.begin();
	
	while (entIt != exitEnts.end()) 
	{
		RectArea2D* externArea = (*entIt)->getMngr()->getComponent<RectArea2D>(*entIt);

		if (externArea != nullptr) 
		{
			if (area->overlapsWithArea(externArea)) {
				unchangedEnts.push_back(*entIt);
				exitEnts.erase(entIt++);
			}
			else {	
				++entIt;
			}
		}
	}
	
	updateOverlappingEntities(); // Update overlapping entity list

	std::list<ecs::entity_t> enteredEnts; // Obtain entered entities

	std::set_difference(
		_currentOverlappingEntities.begin(), _currentOverlappingEntities.end(), unchangedEnts.begin(), unchangedEnts.end(),
		std::inserter(enteredEnts, enteredEnts.begin()));
		
	_triggerState = { exitEnts, unchangedEnts, enteredEnts }; // Update the TriggerState
}

void TriggerComponent::handleAreaOverlapping()
{
	updateTriggerState();

	if (!_triggerState.enteredEntities.empty())
	{
		auto& callbacks = _eventConnections.at(TriggerComponent::AREA_ENTERED);

		for (CALLBACK callback : callbacks) {
			_callbackContextEntities = _triggerState.enteredEntities;
			callback();
		}
			
	}

	if (!_triggerState.exitedEntities.empty())
	{
		auto& callbacks = _eventConnections.at(TriggerComponent::AREA_LEFT);

		for (CALLBACK callback : callbacks) {
			_callbackContextEntities = _triggerState.enteredEntities;
			callback();
		}
	}
}

void TriggerComponent::update()
{
	handleMouseInput();
	handleAreaOverlapping();
}