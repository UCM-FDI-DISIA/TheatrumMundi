#pragma once
#include "ScenePuzzleTemplate.h"

class ParrotPuzzleScene : public ScenePuzzleTemplate
{
	ParrotPuzzleScene();
	~ParrotPuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
};

