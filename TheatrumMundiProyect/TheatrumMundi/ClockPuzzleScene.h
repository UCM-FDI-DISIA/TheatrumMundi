#pragma once
#include "ScenePuzzleTemplate.h"

class ClockPuzzleScene : public ScenePuzzleTemplate
{
public:
	ClockPuzzleScene();
	~ClockPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void refresh() override;
	void unload() override;
	bool Check() override;

private:
	int _actualHour;
	int _actualMinute;
};

