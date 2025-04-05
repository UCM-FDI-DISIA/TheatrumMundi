#pragma once
#include "SceneRoomTemplate.h"

#include "../TheatrumMundi/AudioManager.h"

class ScrollComponent;

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
		BooksPuzzleScn,
		ClockPuzzleSnc,
		TeaCupPuzzleSnc,
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
	void unload() override;
	

private:
	void endDialogue()override;
	void _setRoomEvents();
	void _setRoomAudio();
	void _setGlobalFeatures();

	void _setRoomBackground();
	void _setCaseResolution();
	void _setInteractuables();
	void _setDialog();
	void _setUI();
	
	struct roomSounds {
		Sound uiButton;
		Sound puzzleButton;
	} rmSounds;

	struct roomObjects {
		ecs::entity_t quitButton		  = nullptr;
		ecs::entity_t inventoryButton	  = nullptr;
		ecs::entity_t readyToResolveBotton = nullptr;
		ecs::entity_t blackBackground = nullptr;
		ScrollComponent* backgroundScroll = nullptr;
	} rmObjects;

	bool stopAnimation;
	bool alternate;
};

