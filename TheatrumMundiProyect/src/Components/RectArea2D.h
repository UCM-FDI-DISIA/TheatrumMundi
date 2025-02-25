#pragma once
#include "Area2D.h"

#include "../ecs/ecs.h"

class RectArea2D : public Area2D
{
public:
	//__CMPID_DECL__(ecs::cmp::AREA2D) Has same ID as parent

	RectArea2D();
	RectArea2D(int width, int height);
	RectArea2D(Vector2D localPos, int width, int height);
	RectArea2D(Area2DLayerManager* areaLayerMngr);
	RectArea2D(Area2DLayerManager* areaLayerMngr, Vector2D localPos, int width, int height);


	void initComponent() override;

	bool containsPoint(Vector2D point) override;

	bool overlapsWithArea(Area2D* area) override {
		return area->_overlapsWith(this);
	}

	bool _overlapsWith(RectArea2D* rectArea) override;
	bool _overlapsWith(CircleArea2D* rectArea) override;

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