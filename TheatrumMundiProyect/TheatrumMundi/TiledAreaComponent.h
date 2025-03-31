#pragma once
#include "Area2D.h"
#include <vector>
#include <SDL.h>
class Transform;
class TiledAreaComponent : public Area2D
{
private:
	std::vector<std::vector<bool>> matcol;
	std::vector<std::vector<Vector2D>> matcopos;
	Transform* _objTransform;
	int _width;
	int _height;
	int unitw;
	int unith;
protected:

public:
	TiledAreaComponent(Area2DLayerManager* areaLayerMngr, Transform* objTransform, int width, int height, int fil, int col);
	~TiledAreaComponent();

	 bool containsPoint(Vector2D point) override;

	 bool overlapsWithArea(Area2D* area) override;

	 bool _overlapsWith(RectArea2D* rectArea) override;

	 bool _overlapsWith(TiledAreaComponent* area) override;

	 bool _overlapsWith(CircleArea2D* circleArea) override;

	 void setActiveTile(bool t, int i, int j);

	 bool CheckCollisionInTiles(SDL_Rect& _collition);
	 bool CheckCollisionInTilesCircles(const Vector2D& pos,int rad);

	 inline int getWidth() {
		 return _width;
	 }

	 inline int getHeight() {
		 return _height;
	 }
};

