#include "RectArea2D.h"

#include <SDL.h>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "CircleArea2D.h"
#include "../utils/Collisions.h"

RectArea2D::RectArea2D(Area2DLayerManager* areaLayerMngr, Vector2D localPos, int width, int height)
	: Area2D(areaLayerMngr, localPos), _width(width), _height(height)
{

}

RectArea2D::RectArea2D(Vector2D localPos, int width, int height)
	: Area2D(localPos), _width(width), _height(height)
{

}

RectArea2D::RectArea2D(Area2DLayerManager* areaLayerMngr) 
	: RectArea2D(areaLayerMngr, Vector2D(), -1, -1)
{

}

RectArea2D::RectArea2D(int width, int height) : RectArea2D(Vector2D(), width, height)
{

}

RectArea2D::RectArea2D() : RectArea2D(Vector2D(), -1, -1)
{

}

void RectArea2D::initComponent()
{
	if (_width != -1 && _height != -1) return;

	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return; 
	
	// Automatic scaling
	_width = transform->getWidth();
	_height = transform->getHeight();
}

bool RectArea2D::containsPoint(Vector2D point)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return false;

	int wordX = transform->getPos().getX() +_localPosition.getX();
	int wordY = transform->getPos().getY() +_localPosition.getY();

	SDL_Rect rect = { 
		wordX,
		wordY,
		_width,
		_height
	};

	SDL_Point pnt = { point.getX(), point.getY() };

	return SDL_PointInRect(&pnt, &rect);
}

bool RectArea2D::overlapsWith(RectArea2D* rectArea)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);
	Transform* extrentTr = rectArea->getContext()->getMngr()->getComponent<Transform>(rectArea->getContext());

	if (transform == nullptr || extrentTr == nullptr) return false;

	SDL_Rect thisRect = {
		transform->getPos().getX() + _localPosition.getX(),
		transform->getPos().getY() + _localPosition.getY(),
		_width,
		_height
	};

	SDL_Rect otherRect = {
		extrentTr->getPos().getX() + rectArea->getLocalPos().getX(),
		extrentTr->getPos().getY() + rectArea->getLocalPos().getY(),
		rectArea->getWidth(),
		rectArea->getHeight()
	};

	SDL_Rect rs;
	return SDL_IntersectRect(&thisRect, &otherRect, &rs);
}

bool RectArea2D::overlapsWith(CircleArea2D* circleArea)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);
	Transform* extrentTr = circleArea->getContext()->getMngr()->getComponent<Transform>(circleArea->getContext());
	
	if (transform == nullptr || extrentTr == nullptr) return false;

	return Collisions::rectCollidesWithCircle(
		transform->getPos() + _localPosition, _width, _height,
		extrentTr->getPos() + circleArea->getLocalPos(), circleArea->getRadius()
	);
}