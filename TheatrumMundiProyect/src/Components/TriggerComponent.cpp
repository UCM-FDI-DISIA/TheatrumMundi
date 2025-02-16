#include "TriggerComponent.h"

#include "../sdlutils/InputHandler.h"
#include "Area2D.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

TriggerComponent::TriggerComponent()
{
	_eventConnections.insert({ JUST_ENTERED, {} });
	_eventConnections.insert({ JUST_LEFT, {} });
}

void TriggerComponent::handleMouseInput()
{
	if (!ih().mouseMotionEvent()) return;
	
	Area2D* area = _ent->getMngr()->getComponent<Area2D>(_ent);
	if (area == nullptr) return; // Component needs Area2D to check the event
	
	bool mouseMovedInside = areaIsHovered(ih().getMousePos());
	
	if (mouseMovedInside && !_mouseIsInside)
	{
		_mouseIsInside = true;

		auto& callbacks = _eventConnections.at(TriggerComponent::JUST_ENTERED);

		for (CALLBACK callback : callbacks)
			callback();
	}
	else if (!mouseMovedInside && _mouseIsInside)
	{
		_mouseIsInside = false;

		auto& callbacks = _eventConnections.at(TriggerComponent::JUST_LEFT);

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
	Area2D* area = _ent->getMngr()->getComponent<Area2D>(_ent);
	if (area == nullptr) return false;

	return area->containsPoint(Vector2D(mousePos.first, mousePos.second));
}

void TriggerComponent::update()
{
	handleMouseInput();
}