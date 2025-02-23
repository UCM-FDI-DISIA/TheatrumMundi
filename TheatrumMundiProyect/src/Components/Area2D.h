#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../ecs/ecs.h"
#include "../../TheatrumMundi/Area2DLayerManager.h"

class RectArea2D;
class CircleArea2D;

class Area2D : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::AREA2D)

	Area2D() : Area2D(Vector2D(0, 0)) {}

	Area2D(Vector2D localPosition) : _localPosition(localPosition) {}

	Area2D(Area2DLayerManager* areaLayerMngr, Vector2D localPosition);

	~Area2D();

	bool pointIsOverlayered(Vector2D point);

	virtual bool containsPoint(Vector2D point) = 0;

	// This method will call area->overlapsWith(this) in the subclases to
	// guess in runtime what is the derived type of Area2D pased to calculate
	// the overlap with the correct combination (Rect v Circl | Rect v Rect ...)
	virtual bool overlapsWithArea(Area2D* area) = 0; 

	virtual bool overlapsWith(RectArea2D* rectArea) = 0;
	virtual bool overlapsWith(CircleArea2D* rectArea) = 0;

	void setLocalPos(Vector2D pos) {
		_localPosition = pos;
	}

	Vector2D getLocalPos() {
		return _localPosition;
	}

protected:

	Vector2D _localPosition;
	
	// Reference tor the Area2DLayerManager, that hold the list of all the Area2D in the scene
	Area2DLayerManager* _areaLayerMngr = nullptr;  

	// Iterator that gives this area a position in the 2D layer system
	Area2DLayerManager::iterator _areaLayerPos;
};