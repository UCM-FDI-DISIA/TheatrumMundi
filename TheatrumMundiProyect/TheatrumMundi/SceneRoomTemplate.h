#pragma once
#include "SceneTemplate.h"
#include <vector>
#include <functional>
class DialogueManager;
class SceneRoomTemplate: public SceneTemplate
{
protected:

	std::vector<bool> puzzlesol;
	std :: vector<ecs::entity_t> puzzleptr;
	std::vector< std::function<void()>> roomEvent;
	DialogueManager* dm;
	//inventory


	public:
		//void startDialogue(const eventToRead& _eventToRead);
		void endDialogue();
		virtual void resolvedPuzzle(int i);
		void setActiveBottons(bool active);
		void setActiveInteractObj(bool active);
	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

