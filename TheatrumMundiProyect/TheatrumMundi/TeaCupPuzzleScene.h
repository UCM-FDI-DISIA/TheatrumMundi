#pragma once
#include "ScenePuzzleTemplate.h"

class TeaCupPuzzleScene : public ScenePuzzleTemplate
{
public:

	TeaCupPuzzleScene();
	~TeaCupPuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
	bool isItemHand(const std::string& itemId) override;

protected:

	bool _spoonIsInCup;
	bool _poisonIsChecked;
	Entity* spoon;
	Entity* teaCupBackground;
	ecs::entity_t logbtn;
};

