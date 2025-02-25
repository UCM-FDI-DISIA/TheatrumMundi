#include "DragComponent.h"

#include "../sdlutils/InputHandler.h"
#include "Transform.h"
#include "ClickComponent.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../Components/RectArea2D.h"
#include "../Components/CircleArea2D.h"
#include "../../TheatrumMundi/PhysicsBodyComponent.h"

DragComponent::DragComponent()
{
	_eventConnections.insert({ DRAG_START, {} });
	_eventConnections.insert({ DRAG,	   {} });
	_eventConnections.insert({ DRAG_END,   {} });
}

void DragComponent::handleMouseInput()
{
	if (ih().mouseButtonDownEvent())
	{
		bool clickEvent = areaIsClicked(ih().getMousePos());

		if (!clickEvent) return;

		_isBeingClicked = true;

		updateEntityDragPoint(ih().getMousePos());

		auto& callbacks = _eventConnections.at(DragComponent::DRAG_START);

		for (CALLBACK callback : callbacks)
			callback();
	}
	else if (ih().mouseButtonUpEvent())
	{
		bool releaseEvent = _isBeingClicked;

		if (!releaseEvent) return;

		_isBeingClicked = false;

		auto& callbacks = _eventConnections.at(DragComponent::DRAG_END);

		for (CALLBACK callback : callbacks)
			callback();
	}

	if (_isBeingClicked && ih().mouseMotionEvent())
	{
		moveEntity(ih().getMousePos());

		auto& callbacks = _eventConnections.at(DragComponent::DRAG);

		for (CALLBACK callback : callbacks)
			callback();
	}
}

void DragComponent::updateEntityDragPoint(std::pair<Sint32, Sint32> mousePos)
{
	Transform* tr = _ent->getMngr()->getComponent<Transform>(_ent);
	if (tr == nullptr) return;

	_entityDragPoint = Vector2D(mousePos.first, mousePos.second) - tr->getPos();
}

void DragComponent::moveEntity(std::pair<Sint32, Sint32> mousePos) // TODO Use physics movement (for collisions)
{
	Transform* tr = _ent->getMngr()->getComponent<Transform>(_ent);
	if (tr == nullptr) return;
	Vector2D oldPosition = tr->getPos();
	Vector2D newPosition = Vector2D(
		mousePos.first - _entityDragPoint.getX(),
		mousePos.second - _entityDragPoint.getY()
	);
	tr->getPos().set(newPosition);

	//ChecksPhysics from the entity
	PhysicsBodyComponent* phy = _ent->getMngr()->getComponent<PhysicsBodyComponent>(_ent);
	Area2D* area = _ent->getMngr()->getComponent<RectArea2D>(_ent);
	assert(area != nullptr);
	if (phy != nullptr && !phy->CheckAreaColision(area)) {
		tr->getPos().set(oldPosition);
		updateEntityDragPoint(ih().getMousePos());
	}
}