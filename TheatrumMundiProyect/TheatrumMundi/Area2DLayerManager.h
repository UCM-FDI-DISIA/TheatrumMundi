#pragma once
#include <list>

class Area2D;

class Area2DLayerManager
{
public:

	using iterator = std::list<Area2D*>::iterator;
	using reverse_iterator = std::list<Area2D*>::reverse_iterator;

	Area2DLayerManager();
	~Area2DLayerManager();

	// Reference to the Area2D list
	std::list<Area2D*> const& list();
	
	// Inserts the Area2D at the last collision layer
	void pushBack(Area2D* area); 
	
	// Inserts the Area2D at the first collision layer
	void pushFront(Area2D* area); 

	// Inserts the Area2D at the iterator's collision layer
	iterator insert(Area2DLayerManager::iterator layerIt, Area2D* area);

	// Moves one layer back the Area2D
	void moveBack(Area2DLayerManager::iterator layerIt);

	// Moves one layer foward the Area2D
	void moveFoward(Area2DLayerManager::iterator layerIt);

	// Sends the Area2D to the last collision layer
	void sendBack(Area2DLayerManager::iterator layerIt);

	// Sends the Area2D to the first collision layer
	void sendFront(Area2DLayerManager::iterator layerIt);

	// Sends the Area2D to the layer behind the iterator (on below the other area)
	void sendBehind(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt);

	// Sends the Area2D to the layer after the iterator (on top of the other area)
	void sendAfter(Area2DLayerManager::iterator otherAreaLayerIt, Area2DLayerManager::iterator layerIt);

	// Erases the Area2D from the collision list
	void erase(Area2DLayerManager::iterator layerIt);

	Area2DLayerManager::iterator begin() {
		return _areaLayerOrderList.begin();
	}

	Area2DLayerManager::iterator end() {
		return _areaLayerOrderList.end();
	}

	Area2DLayerManager::reverse_iterator rbegin() {
		return _areaLayerOrderList.rbegin();
	}

	Area2DLayerManager::reverse_iterator rend() {
		return _areaLayerOrderList.rend();
	}

protected:

	std::list<Area2D*> _areaLayerOrderList; // List that represents the order of the Area2D components in the scene
};

