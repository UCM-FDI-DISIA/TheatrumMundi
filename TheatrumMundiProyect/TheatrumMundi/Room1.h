#pragma once
#include "SceneRoomTemplate.h"



class Inventory;
class Room1 : public SceneRoomTemplate
{
private:
	Inventory* inv1;
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
		ResolveCase,
		ResolveBottons,
		GoodEnd,
		BadEnd,
		LOGENABLE,
		LOGDESABLE,
		event_size,
	};
public:
	Room1Scene();
	~Room1Scene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;

};

