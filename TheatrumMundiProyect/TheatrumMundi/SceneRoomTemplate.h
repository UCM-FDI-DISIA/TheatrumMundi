#pragma once
#include "SceneTemplate.h"
class SceneRoomTemplate: public SceneTemplate
{public:

	SceneRoomTemplate();
	~SceneRoomTemplate();
	
	void init() override;
	void unload() override;

};

