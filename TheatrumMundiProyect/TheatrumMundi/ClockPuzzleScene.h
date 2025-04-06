#pragma once
#include "ScenePuzzleTemplate.h"

class ClockPuzzleScene : public ScenePuzzleTemplate
{
public:
	ClockPuzzleScene();
	~ClockPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool isItemHand(const std::string& itemId) override;
	bool Check() override;
	void Win() override;
	void ResolveScene() override;
private:
	int _actualHour;
	int _actualMinute;
	bool hasLongClockHand; //if the cloack have the minute hand
	bool hasShortClockHand; //if the cloack have the hour hand
	Entity* clock;
	Entity* longClockHand;
	Entity* shortClockHand;
	ecs::entity_t logbtn;
};

