#include "Area2DLayerManager.h"
//#define _ITERATOR_DEBUG_LEVEL 2
#include "../src/Components/Area2D.h"
#include <iterator>

Area2DLayerManager::Area2DLayerManager()
{

}

std::list<Area2D*> const& Area2DLayerManager::list()
{
	return _areaLayerOrderList;
}

void Area2DLayerManager::pushBack(Area2D* area)
{
	_areaLayerOrderList.push_back(area);
}

void Area2DLayerManager::pushFront(Area2D* area)
{
	_areaLayerOrderList.push_front(area);
}

Area2DLayerManager::iterator Area2DLayerManager::insert(Area2DLayerManager::iterator layerIt, Area2D* area)
{
	return _areaLayerOrderList.insert(layerIt, area);
}

void Area2DLayerManager::moveBack(Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.begin()) return;

	_areaLayerOrderList.splice(std::prev(layerIt, 1), _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::moveFoward(Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.splice(std::next(layerIt, 1), _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::sendBack(Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.splice(_areaLayerOrderList.end(), _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::sendFront(Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.splice(_areaLayerOrderList.begin(), _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::sendBehind(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.splice(otherAreaLayerIt, _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::sendAfter(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt)
{
	if (otherAreaLayerIt == _areaLayerOrderList.end()) return;
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.splice(++otherAreaLayerIt, _areaLayerOrderList, layerIt);
}

void Area2DLayerManager::erase(Area2DLayerManager::iterator layerIt)
{
	if (layerIt == _areaLayerOrderList.end()) return;

	_areaLayerOrderList.erase(layerIt);
}