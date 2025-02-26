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

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

