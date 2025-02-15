#include "CircleArea2D.h"

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"

CircleArea2D::CircleArea2D(int radius) : _radius(radius)
{

}

CircleArea2D::CircleArea2D() : CircleArea2D(0)
{

}

void CircleArea2D::initComponent()
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return;

	_radius = transform->getWidth() / 2;
}

bool CircleArea2D::containsPoint(Vector2D point)
{
	Transform* transform = _ent->getMngr()->getComponent<Transform>(_ent);

	if (transform == nullptr) return false;

	int wordX = transform->getPos().getX() + _localPosition.getX();
	int wordY = transform->getPos().getY() + _localPosition.getY();

	int centerDist = sqrt(
		pow(abs(point.getX() - wordX), 2)
		+ pow(abs(point.getY() - wordY), 2)
	);

	return centerDist <= _radius;
}