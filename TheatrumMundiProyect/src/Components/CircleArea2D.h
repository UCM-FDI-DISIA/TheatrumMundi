#pragma once
#include "Area2D.h"

#include "../ecs/ecs.h"

class CircleArea2D : public Area2D
{
public:
	//__CMPID_DECL__(ecs::cmp::CIRCLEAREA2D);

	CircleArea2D();
	CircleArea2D(int radius);
	CircleArea2D(Vector2D localPos, int radius);

	void initComponent() override;

	bool containsPoint(Vector2D point) override;

	bool overlapsWithArea(Area2D* area) override {
		return area->overlapsWith(this);
	}

	bool overlapsWith(RectArea2D* rectArea) override;
	bool overlapsWith(CircleArea2D* rectArea) override;

	RectArea2D* GetRectArea() override;

	CircleArea2D* GetCircleArea() override;

	void setRadius(int value) {
		_radius = value;
	}

	int getRadius() {
		return _radius;
	}

protected:

	int _radius;
};