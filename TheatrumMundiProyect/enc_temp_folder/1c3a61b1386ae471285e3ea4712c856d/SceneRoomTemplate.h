#pragma once
#include "SceneTemplate.h"
#include <vector>
#include "EventsInfo.h"
#include <functional>
class SceneRoomTemplate: public SceneTemplate
{
protected:

	std::vector<bool> puzzlesol;
	std :: vector<ecs::entity_t> puzzleptr;
	std::vector< std::function<void()>> roomEvent;
	ecs::entity_t body;
	bool logActive = false;
	//inventory


	public:
		void startDialogue(const eventToRead& _eventToRead);
		void endDialogue();
		//use the room enum for de index
		virtual void resolvedPuzzle(int i);
		void setActiveBottons(bool active);
		void setActiveZoomObj(bool active);
	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

