#pragma once
#include "ScenePuzzleTemplate.h"
class DoorPuzzleScene : public ScenePuzzleTemplate
{
public:
	DoorPuzzleScene();
	~DoorPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;
};

