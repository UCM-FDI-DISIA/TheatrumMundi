#pragma once
#include "ScenePuzzleTemplate.h"
class SceneRoomTemplate;

class BalancePuzzleScene:public ScenePuzzleTemplate
{

public:

	BalancePuzzleScene();
	~BalancePuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void init() override;
	void unload() override;
	bool Check() override;
	bool isItemHand(const std::string& itemId) override;

protected:

	bool _featherIsInBalance;
	//bool _poisonIsChecked;
	Entity* feather;
	Entity* balanceBackground;
};

