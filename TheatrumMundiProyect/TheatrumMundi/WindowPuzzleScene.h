#pragma once
#include "ScenePuzzleTemplate.h"
class WindowPuzzleScene : public ScenePuzzleTemplate
{
private:
	//Rope and OpenWindow
	bool isRopped; //If the window has the rope
	ecs::entity_t rope;
	ecs::entity_t openWindow;
public:
	WindowPuzzleScene();
	~WindowPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;
};

