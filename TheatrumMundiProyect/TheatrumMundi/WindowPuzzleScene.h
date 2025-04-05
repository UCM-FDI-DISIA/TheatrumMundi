#pragma once
#include "ScenePuzzleTemplate.h"
class WindowPuzzleScene : public ScenePuzzleTemplate
{
public:
	WindowPuzzleScene();
	~WindowPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;
};

