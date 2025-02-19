#pragma once

#include "SceneRoomTemplate.h"



class DebugLogRoom : public SceneRoomTemplate
{
private:
	bool logActive = false;
protected:
public:
	DebugLogRoom();
	~DebugLogRoom();
	void init() override;
	void virtual refresh() override;
	void virtual unload() override;
	

};
