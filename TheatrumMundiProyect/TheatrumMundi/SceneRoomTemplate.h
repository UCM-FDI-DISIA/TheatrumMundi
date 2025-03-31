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
	//ecs::entity_t readyToResolveBotton;
	bool finishallpuzzles;
	bool logActive = false;
	entity_t characterCorpse;
	//inventory


	public:
		//use the room enum for de index
		virtual void resolvedPuzzle(int i)=0;
		void setActiveBottons(bool active);
		void setActiveZoomObj(bool active);
		inline Inventory* GetInventory() { return inv; }
		/// <summary>
		/// Scroll the inventory up or down depending on the direction
		/// </summary>
		/// <param name="dir"></param>
		void scrollInventory(int dir);

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
};

