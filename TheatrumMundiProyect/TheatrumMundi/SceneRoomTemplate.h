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

	//inventory


	public:
		void startDialogue(const eventToRead& _eventToRead);
		void endDialogue();
		virtual void resolvedPuzzle(int i);
		void setActiveBottons(bool active);
		void setActiveInteractObj(bool active);
	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

