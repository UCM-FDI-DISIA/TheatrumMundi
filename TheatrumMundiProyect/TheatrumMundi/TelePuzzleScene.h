#pragma once
#include "ScenePuzzleTemplate.h"

class TelePuzzleScene : public ScenePuzzleTemplate
{
public:

	TelePuzzleScene();
	~TelePuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void refresh() override;
	void unload() override;
	bool Check() override;
	bool isItemHand(const std::string& itemId) override;

protected:

	bool _spoonIsInCup;
	bool _poisonIsChecked;
	Entity* spoon;
	Entity* teaCupBackground;

private:
	ecs::entity_t inventoryButton;
};