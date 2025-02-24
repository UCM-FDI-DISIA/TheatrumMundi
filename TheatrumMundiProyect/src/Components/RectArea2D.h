#pragma once
#include "Area2D.h"

#include "../ecs/ecs.h"

class RectArea2D : public Area2D
{
public:
	//__CMPID_DECL__(ecs::cmp::RECTAREA2D) 

	RectArea2D();
	RectArea2D(int width, int height);
	RectArea2D(Vector2D localPos, int width, int height);

	void initComponent() override;

	bool containsPoint(Vector2D point) override;

	bool overlapsWithArea(Area2D* area) override {
		return area->overlapsWith(this);
	}

	bool overlapsWith(RectArea2D* rectArea) override;
	bool overlapsWith(CircleArea2D* rectArea) override;

	RectArea2D* GetRectArea() override;

	CircleArea2D* GetCircleArea() override;

	void setWidth(int value) {
		_width = value;
	}

	void setHeight(int value) {
		_height = value;
	}

	int getWidth() {
		return _width;
	}

	int getHeight() {
		return _height;
	}

protected:

	int _width;
	int _height;
};