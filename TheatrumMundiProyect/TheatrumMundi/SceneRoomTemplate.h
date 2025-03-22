#pragma once
#include "SceneTemplate.h"
#include <vector>

#include <functional>
class SceneRoomTemplate: public SceneTemplate
{
protected:

	Inventory* inv;
	std::vector<bool> puzzlesol;
	/// <summary>
	/// Vector of pointer to puzzle entities of scene
	/// </summary>
	std :: vector<ecs::entity_t> puzzleptr;
	/// <summary>
	/// Vector of lambda funtions of events
	/// </summary>
	std::vector< std::function<void()>> roomEvent;
	ecs::entity_t body;
	/// <summary>
	/// Bool to control if all puzzles have been resolved
	/// </summary>
	bool finishallpuzzles;
	bool logActive = false;
	entity_t body;
	//inventory


	public:
		//use the room enum for de index
		virtual void resolvedPuzzle(int i)=0;
		void setActiveBottons(bool active);
		void setActiveZoomObj(bool active);
		inline Inventory* GetInventory() { return inv; }

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

