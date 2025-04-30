#pragma once
#include "SceneTemplate.h"
#include "ecs.h"
#include "../src/components/WriteTextComponent.h"
class CreditsScene : public SceneTemplate
{
private:
	std::vector<Texture*>courtainAnimationFrames;
	bool closedSecondTime;
	//VirtualTimer frameTimer;
	bool isAnimating;
	int dialogCount;
	int currentFrameIndex;
	Entity* courtainsBackground;

public:
	CreditsScene();
	~CreditsScene();
	void init() override;
	void openCourtains();
	void closeCourtains();
};

