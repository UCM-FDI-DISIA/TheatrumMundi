#include "MouseIntaractionComponent.h"
#include "../sdlutils/SDLUtils.h"

MouseIntaractionComponent::MouseIntaractionComponent()
{
}

MouseIntaractionComponent::~MouseIntaractionComponent()
{
}

bool MouseIntaractionComponent::connect(EVENT_TYPE eventType, CALLBACK action)
{
	auto eventHashIt = _eventConnections.find(eventType);
	if (eventHashIt == _eventConnections.end()) return false;

	eventHashIt->second.push_back(action);

	return true;
}

void MouseIntaractionComponent::update()
{
	handleMouseInput();
}