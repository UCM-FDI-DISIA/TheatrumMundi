#include "Area2D.h"

Area2D::Area2D(Area2DLayerManager* areaLayerMngr, Vector2D localPosition)
	: _areaLayerMngr(areaLayerMngr), _localPosition(localPosition)
{
	areaLayerMngr->pushFront(this);
	_areaLayerPos = areaLayerMngr->begin();
}

Area2D::~Area2D() {
	_areaLayerMngr->erase(_areaLayerPos);
}
#include <iostream>
bool Area2D::pointIsOverlayered(Vector2D point)
{
	if (_areaLayerPos == _areaLayerMngr->begin()) return false;

	bool overlayered = false;
	Area2DLayerManager::reverse_iterator areaRvIt(_areaLayerPos);
	
	while (areaRvIt != _areaLayerMngr->rend() && !overlayered)
	{
		overlayered = (*areaRvIt)->containsPoint(point); 
		++areaRvIt;
	}

	return overlayered;
}