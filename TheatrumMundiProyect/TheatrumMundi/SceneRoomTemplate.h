#pragma once
#include "SceneTemplate.h"
#include <vector>
class DialogueManager;
class SceneRoomTemplate: public SceneTemplate
{
protected:

	std::vector<bool> puzzlesol;
	std :: vector<ecs::entity_t> puzzleptr;
	DialogueManager* dm;
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

