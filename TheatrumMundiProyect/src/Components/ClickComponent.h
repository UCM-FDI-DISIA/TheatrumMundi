#pragma once
#include "MouseIntaractionComponent.h"

#include <SDL.h>

class ClickComponent : public MouseIntaractionComponent
{
public:

	enum EventType { JUST_CLICKED, JUST_RELEASED };

	ClickComponent();

	void update() override;

protected:

	void handleMouseInput();

	bool areaIsClicked(std::pair<Sint32, Sint32> mousePos);
};

