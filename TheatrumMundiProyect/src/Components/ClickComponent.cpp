#include "ClickComponent.h"

#include "../sdlutils/InputHandler.h"

ClickComponent::ClickComponent()
{
	_eventConnections.insert({ JUST_CLICKED, {} });
	_eventConnections.insert({ JUST_RELEASED, {} });
}

void ClickComponent::handleMouseInput()
{
	if (ih().mouseButtonDownEvent())
	{
		bool clickEvent = areaIsClicked(ih().getMousePos());

		if (!clickEvent) return;

		_isBeingClicked = true;

		auto& callbacks = _eventConnections.at(ClickComponent::JUST_CLICKED);

		for (CALLBACK callback : callbacks)
			callback();
	}
	else if (ih().mouseButtonUpEvent())
	{
		bool releaseEvent = _isBeingClicked;

		if (!releaseEvent) return;

		_isBeingClicked = false;

		auto& callbacks = _eventConnections.at(ClickComponent::JUST_RELEASED);

		for (CALLBACK callback : callbacks)
			callback();
	}
}

bool ClickComponent::areaIsClicked(std::pair<Sint32, Sint32> mousePos)
{
	return true;
}

void ClickComponent::update()
{
	handleMouseInput();
}