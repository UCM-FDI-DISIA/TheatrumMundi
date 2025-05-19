#pragma once
#include "ScenePuzzleTemplate.h"
class RavenPuzzleScene : public ScenePuzzleTemplate
{
private:
	bool ravenHappy; //If the player change the Jewel with the player or not
	ecs::entity_t raven;
	ecs::entity_t key;
public:

	RavenPuzzleScene();
	~RavenPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool isItemHand(const std::string& itemId) override;
	void Win() override;

};

