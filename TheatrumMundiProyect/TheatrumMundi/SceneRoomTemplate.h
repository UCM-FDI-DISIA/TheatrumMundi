#pragma once
#include "SceneTemplate.h"

#include <vector>

#include <functional>
class RectArea2D;
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
	
	/// <summary>
	/// Bool to control if all puzzles have been resolved
	/// </summary>
	bool finishallpuzzles;
	bool logActive = false;

	entity_t characterCorpse;

	
	//InventoryObjects
	struct InventoryObjects {
		ecs::entity_t InventoryBackground = nullptr;
		RectArea2D* InvArea = nullptr;
		ecs::entity_t inventoryUpButton = nullptr;
		ecs::entity_t inventoryDownButton = nullptr;

	}invObjects;

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
		void scrollInventory(int dir); //scroll room1
		void HideAllInvetoryItems(const ecs::entity_t& invBack, const ecs::entity_t& UpButton, const ecs::entity_t& DownButt); //Hide the inventory things

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
};

