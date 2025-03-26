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
		ResolveCase,
		ResolveBottons,
		GoodEnd,
		BadEnd,
		MobileDialogue,
		LOGENABLE,
		LOGDESABLE,
		EVENTS_SIZE,
	};

public:

	//friend PipePuzzleScene;
	Room1Scene();
	~Room1Scene();

	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;

private:

	void _setRoomEvents();
	void _setRoomAudio();
	void _setGlobalFeatures();

	void _setRoomBackground();
	void _setDialog();
	void _setUI();

	struct {
		Sound uiButton;
		Sound puzzleButton;
	} rmSounds;

	struct {
		ecs::entity_t corpseQuitButton;
		ecs::entity_t pauseButton;
		ecs::entity_t inventoryButton;

		ecs::entity_t backgroundScroll;
	} rmObjects;
};

