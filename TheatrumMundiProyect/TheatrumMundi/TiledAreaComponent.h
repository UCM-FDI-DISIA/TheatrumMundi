#pragma once
#include "Area2D.h"
#include <vector>
class Transform;
class TiledAreaComponent : public Area2D
{
private:
	std::vector<std::vector<bool>> matcol;
	std::vector<std::vector<Vector2D>> matcopos;
	Transform* _objTransform;
	int _width;
	int _height;
protected:

public:
	TiledAreaComponent(Area2DLayerManager* areaLayerMngr, Transform* objTransform, int width, int height, int fil, int col);
	~TiledAreaComponent();

	 bool containsPoint(Vector2D point) override;

	 bool overlapsWithArea(Area2D* area) override;

	 bool _overlapsWith(RectArea2D* rectArea) override;

	 bool _overlapsWith(CircleArea2D* rectArea) override;

	 void setActiveTile();
};

