#pragma once
#include "SceneRoomTemplate.h"

class TutorialScene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;
protected:
	enum TutorialEvent
	{
		InitialDialogue = 0,
		ClockPuzzleSnc,
		ClockPuzzleRsv,
		teleScene,
		Spoon,
		ResolveCase,
		ResolveBottons,
		MobileDialogue,
		LOGENABLE,
		LOGDESABLE,
		event_size,
	};
public:
	TutorialScene();
	~TutorialScene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;

};

