#pragma once
#include "MouseIntaractionComponent.h"

#include <SDL.h>

class TriggerComponent : public MouseIntaractionComponent
{
public:
	__CMPID_DECL__(ecs::cmp::TRIGGER_COMPONENT)

	enum EventType { JUST_ENTERED, JUST_LEFT };

	TriggerComponent();

	bool mouseIsIn();

protected:

	void handleMouseInput() override;

	bool areaIsHovered(std::pair<Sint32, Sint32> mousePos);

	bool _mouseIsInside = false;
};