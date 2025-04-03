#pragma once
#include "SceneRoomTemplate.h"

class Room1Scene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;
protected:
	enum Room1Event
	{
		InitialDialogue = 0,
		CorpseDialogue,
		PipePuzzleSnc,
		PipePuzzleRsv,
		BooksPuzzleScn,
		BooksPuzzleRsv,
		ClockPuzzleSnc,
		ClockPuzzleRsv,
		TeaCupPuzzleSnc,
		TeaCupPuzzleRsv,
		Spoon,
		Boa1,
		Boa2,
		Hanni,
		//Doku,
		ResolveCase,
		ResolveBottons,
		GoodEnd,
		BadEnd,
		MobileDialogue,
		LOGENABLE,
		LOGDESABLE,
		event_size,
	};
public:
	//friend PipePuzzleScene;
	Room1Scene();
	~Room1Scene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;

};

