#pragma once
#include "SceneTemplate.h"
#include <vector>
#include "EventsInfo.h"
class SceneRoomTemplate: public SceneTemplate
{
protected:

	std::vector<bool> puzzlesol;
	std :: vector<ecs::entity_t> puzzleptr;
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

