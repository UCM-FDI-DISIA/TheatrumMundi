#include "MouseIntaractionComponent.h"
#include "../sdlutils/SDLUtils.h"

MouseIntaractionComponent::MouseIntaractionComponent()
{
}

MouseIntaractionComponent::~MouseIntaractionComponent()
{
}

void MouseIntaractionComponent::connect(EVENT_TYPE eventType, CALLBACK action)
{
	_eventConnections.at(eventType).push_back(action);
}