#include "ClickComponent.h"

#include "../sdlutils/InputHandler.h"

ClickComponent::ClickComponent()
{
}

void ClickComponent::handleMouseInput()
{
	if (ih().mouseButtonDownEvent())
	{
		bool clickEvent = areaIsClicked(ih().getMousePos());

		if (!clickEvent) return;

		auto& callbacks = _eventConnections.at(ClickComponent::JUST_CLICKED);

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