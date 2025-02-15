#include "RectArea2D.h"

#include <SDL.h>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"

RectArea2D::RectArea2D(int width, int height) : _width(width), _height(height)
{

}

RectArea2D::RectArea2D() : RectArea2D(0, 0)
{

}

void RectArea2D::initComponent()
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return;

	_width = transform->getWidth();
	_height = transform->getHeight();
}

bool RectArea2D::containsPoint(Vector2D point)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return false;

	int wordX = transform->getPos().getX() + _localPosition.getX();
	int wordY = transform->getPos().getY() + _localPosition.getY();

	SDL_Rect rect = { 
		wordX,
		wordY,
		_width,
		_height
	};

	SDL_Point pnt = { point.getX(), point.getY() };

	return SDL_PointInRect(&pnt, &rect);
}