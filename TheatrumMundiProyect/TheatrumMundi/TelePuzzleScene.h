#pragma once
#include "ScenePuzzleTemplate.h"
#include "vector"
#include "../src/sdlutils/VirtualTimer.h"

class TelePuzzleScene : public ScenePuzzleTemplate
{
public:

	TelePuzzleScene();
	~TelePuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void refresh() override;
	void unload() override;
	bool Check() override;
	bool isItemHand(const std::string& itemId) override;

protected:

	bool _spoonIsInCup;
	bool _poisonIsChecked;
	Entity* spoon;
	Entity* tvBackground;

private:
	ecs::entity_t inventoryButton;

	std::vector<Texture*> tvAnimationFrames;
	int currentFrameIndex = 0;
	VirtualTimer frameTimer;
	bool isAnimating = false;
};