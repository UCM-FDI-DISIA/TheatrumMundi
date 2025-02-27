#pragma once
#include "ClickComponent.h"

#include "../utils/Vector2D.h"

class DragComponent : public ClickComponent
{
public:
	__CMPID_DECL__(ecs::cmp::DRAG_COMPONENT)

	enum EventType { DRAG_START, DRAG, DRAG_END };

	DragComponent();

private:

	void updateEntityDragPoint(std::pair<Sint32, Sint32> mousePos);

protected:

	void handleMouseInput() override;

	void moveEntity(std::pair<Sint32, Sint32> mousePos);

	Vector2D _entityDragPoint;
};