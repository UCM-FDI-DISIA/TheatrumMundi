#pragma once
#include "SceneTemplate.h"
#include <vector>

#include <functional>
class SceneRoomTemplate: public SceneTemplate
{
protected:
	/// <summary>
	/// vector of the resolve puzzles in this Room
	/// </summary>
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
	bool finishalpuzzles;
	bool logActive = false;
	//inventory


	public:
		//use the room enum for de index
		virtual void resolvedPuzzle(int i)=0;
		void setActiveBottons(bool active);
		void setActiveZoomObj(bool active);
	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

