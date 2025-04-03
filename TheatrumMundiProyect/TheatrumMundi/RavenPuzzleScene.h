#pragma once
#include "ScenePuzzleTemplate.h"
#include "SceneRoomTemplate.h"
class RavenPuzzleScene : public ScenePuzzleTemplate
{
public:

	RavenPuzzleScene();
	~RavenPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool Check() override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;

};

