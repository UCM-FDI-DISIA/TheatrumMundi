#pragma once
#include "ScenePuzzleTemplate.h"

class LockerPuzzle: public ScenePuzzleTemplate
{
private:
	std::vector<int> rotSol;
	std::vector<Transform*> wheelstr;
	std::vector<ecs::entity_t> doorEntities;
	std::vector<ecs::entity_t> rewardEntities;
protected:
public:
	LockerPuzzle();
	~LockerPuzzle();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
	void Win() override;
	void ResolveScene() override;
};

