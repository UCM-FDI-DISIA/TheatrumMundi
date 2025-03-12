#pragma once
#include "MouseIntaractionComponent.h"

#include <SDL.h>

class ClickComponent : public MouseIntaractionComponent
{
public:
	__CMPID_DECL__(ecs::cmp::CLICK_COMPONENT)

	enum EventType { JUST_CLICKED, JUST_RELEASED };

	ClickComponent();

	inline bool isBeingClicked() {return _isBeingClicked;}

protected:

	void handleMouseInput() override;

	bool areaIsClicked(std::pair<Sint32, Sint32> mousePos);

	bool _isBeingClicked = false;
};

