#pragma once
#include "SceneTemplate.h"
#include <vector>
class SceneRoomTemplate: public SceneTemplate
{
protected:

	std::vector<bool> puzzlesol;
	//inventory


	public:

	SceneRoomTemplate();
	virtual ~SceneRoomTemplate();
	
	

};

