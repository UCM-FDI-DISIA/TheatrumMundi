#pragma once
#include "ScenePuzzleTemplate.h"

class TeaCupPuzzleScene : public ScenePuzzleTemplate
{
public:

	TeaCupPuzzleScene();
	~TeaCupPuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void refresh() override;
	void unload() override;
	bool Check() override;

protected:

	bool _spoonIsInCup;
	bool _poisonIsChecked;
};

