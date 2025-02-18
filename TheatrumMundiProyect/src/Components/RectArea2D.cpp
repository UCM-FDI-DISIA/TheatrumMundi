#include "RectArea2D.h"

#include <SDL.h>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"

RectArea2D::RectArea2D(Vector2D localPos, int width, int height)
	: Area2D(localPos), _width(width), _height(height)
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

bool RectArea2D::overlapsWithArea(Area2D* area)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return false;

	Transform* extrentTr = area->getContext()->getMngr()->getComponent<Transform>(area->getContext());

	int wordX = (transform->getPos().getX() - extrentTr->getPos().getX()) + transform->getWidth() / 2;
	int wordY = transform->getPos().getY() - extrentTr->getPos().getY() + transform->getHeight() / 2;

	int centerDist = sqrt(
		pow(abs(wordX), 2)
		+ pow(abs(wordY), 2)
	);

	return centerDist <= transform->getWidth()/2;
}