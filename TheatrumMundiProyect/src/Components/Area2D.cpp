#include "Area2D.h"

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

Area2D::Area2D(Area2DLayerManager* areaLayerMngr, Vector2D localPosition)
	: _areaLayerMngr(areaLayerMngr), _localPosition(localPosition)
{
	areaLayerMngr->pushFront(this);
	_areaLayerPos = areaLayerMngr->begin();
}

Area2D::~Area2D()
{
	if (_areaLayerMngr == nullptr) return;

	_areaLayerMngr->erase(_areaLayerPos);
}

bool Area2D::pointIsOverlayered(Vector2D point)
{
	if (_areaLayerMngr == nullptr) return false;

	if (_areaLayerPos == _areaLayerMngr->begin()) return false;

	bool overlayered = false;
	Area2DLayerManager::reverse_iterator areaRvIt(_areaLayerPos);
	
	while (areaRvIt != _areaLayerMngr->rend() && !overlayered)
	{
		bool entityIsActive = (*areaRvIt)->getContext()->getMngr()->isActive((*areaRvIt)->getContext());

		overlayered = (*areaRvIt)->containsPoint(point) && entityIsActive;
		++areaRvIt;
	}

	return overlayered;
}