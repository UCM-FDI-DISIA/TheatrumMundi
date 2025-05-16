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
	void Win() override;
	void ResolveScene() override;

protected:
	bool _hasFeather;
	bool _featherIsInBalance;
	Entity* feather;
	Entity* balanceBackground;
};

