#pragma once
#include "SceneTemplate.h"
#include "ecs.h"
#include "../src/components/WriteTextComponent.h"
#include <string>
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
	std::string cred1;
	std::string cred2;
	ecs::Entity* _scrollAreaRight;
	ecs::Entity* _scrollAreaLeft;
	ecs::Entity* _bigCourtain;
	ecs::Entity* _courtainLeft;
	ecs::Entity* _courtainRight;
	bool _backToMainMenu;
public:
	CreditsScene();
	~CreditsScene();
	void init() override;
	void openCourtains();
	void closeCourtains();
	void backToMenu();
};

