#pragma once

#include "SceneRoomTemplate.h"

class Log;

class DebugLogRoom : public SceneRoomTemplate
{
private:
	Log* _sceneLog;
protected:
public:
	DebugLogRoom();
	~DebugLogRoom();
	void init() override;
	void virtual refresh() override;
	void virtual unload() override;

};
