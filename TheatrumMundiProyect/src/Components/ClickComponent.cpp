#include "ClickComponent.h"

#include "InputHandler.h"
#include "Transform.h"
#include "Manager.h"
#include "RectArea2D.h"

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
	Area2D* area = _ent->getMngr()->getComponent<Area2D>(_ent);
	if (area == nullptr) return false;

	return area->containsPoint(Vector2D(mousePos.first, mousePos.second))
		&& !(_hasLayerOpposition && area->pointIsOverlayered(Vector2D(mousePos.first, mousePos.second)))
		&& _ent->getMngr()->isActive(_ent);
}