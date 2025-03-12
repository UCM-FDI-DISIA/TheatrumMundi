#pragma once
#include "ScenePuzzleTemplate.h"

class ClockPuzzleScene : public ScenePuzzleTemplate
{
public:
	ClockPuzzleScene();
	~ClockPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void refresh() override;
	void unload() override;
	bool isCloackHand(const std::string& itemId);
	bool Check() override;
	void Win() override;
private:
	int _actualHour;
	int _actualMinute;
	bool hasLongCloackHand; //if the cloack have the minute hand
	bool hasShortCloackHand; //if the cloack have the hour hand
	bool placeHand; //if an object is on the trigger
	Entity* cloack;
	Entity* longCloackHand;
	Entity* shortCloackHand;
};

