#pragma once
#include "ScenePuzzleTemplate.h"

class ClockPuzzleScene : public ScenePuzzleTemplate
{
public:
	ClockPuzzleScene();
	~ClockPuzzleScene();
	void init() override;
	void refresh() override;
	void unload() override;

private:

};

