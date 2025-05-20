#pragma once
#include "ScenePuzzleTemplate.h"
#include <SDL.h>
#include "../src/sdlutils/VirtualTimer.h"

class Sound;

class CorpseLucyScene : public ScenePuzzleTemplate {
	
public:
	CorpseLucyScene();
	~CorpseLucyScene();

	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
	void refresh() override;

private:
	void _setGlobalFeatures();
	void _setRoomAudio();
	void _setRoomBackground();
	void _setInteractuables(SceneTemplate* sr);
	void _setDialog();
	void _setUI();
	bool isItemHand(const std::string& itemId) override;

	struct roomObjects {
		ecs::entity_t background = nullptr;
		ecs::entity_t interactuableArea = nullptr;
		ecs::entity_t quitButton = nullptr;
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t logbtn = nullptr;
	} rmObjects;

	struct roomSounds {
		std::shared_ptr<Sound> uiButton;
		std::shared_ptr<Sound> puzzleButton;
	} rmSounds;

	bool isAnimating = false;
	VirtualTimer frameTimerCorpse;

};