#pragma once
#include "SceneTemplate.h"
#include <vector>

#include <functional>
class SceneRoomTemplate: public SceneTemplate
{
protected:

	Inventory* inv;
	std::vector<bool> puzzlesol;
	std :: vector<ecs::entity_t> puzzleptr;
	std::vector< std::function<void()>> roomEvent;
	ecs::entity_t body;
	bool finishalpuzzles;
	bool logActive = false;
	//inventory


	public:
		//use the room enum for de index

		virtual void resolvedPuzzle(int i);
		void setActiveBottons(bool active);
		void setActiveZoomObj(bool active);
		inline Inventory* GetInventory() { return inv; }

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

