#pragma once
#include "ScenePuzzleTemplate.h"
class WindowPuzzleScene : public ScenePuzzleTemplate
{
private:
	ecs::entity_t WindowBackGround;
	bool isOpen = false;
	int OpenCount = 0;
public:
	WindowPuzzleScene();
	~WindowPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;
};

