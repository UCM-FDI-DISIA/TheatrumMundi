#pragma once
#include "ScenePuzzleTemplate.h"

class LockerPuzzle: public ScenePuzzleTemplate
{
private:
protected:
public:
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
	void Win() override;
	void ResolveScene() override;
};

