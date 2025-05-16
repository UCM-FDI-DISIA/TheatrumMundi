#pragma once
#include "ScenePuzzleTemplate.h"
#include <SDL.h>

class ParrotPuzzleScene : public ScenePuzzleTemplate
{
public:
	ParrotPuzzleScene();
	~ParrotPuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;

private:

	void _setGlobalFeatures();

	void _setRoomBackground();
	void _setInteractuables();
	void _setDialog();
	void _setUI();

	struct roomObjects {
		ecs::entity_t quitButton = nullptr;
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t logbtn = nullptr;
	} rmObjects;

	struct roomSounds {
		std::shared_ptr<Sound> uiButton;
		std::shared_ptr<Sound> puzzleButton;
		std::shared_ptr<Sound> doorSound;
		std::shared_ptr<Sound> explosionSound;

	} rmSounds;

	struct ParrotUtils {
		ecs::entity_t parrotEnt = nullptr;
		Uint32 lastSoundTime = 0; // The time where the parrot emited the last noise
		std::vector<std::shared_ptr<Sound>> codeSequenceSounds; // Sequence of sounds in the second phase
		int codeSeqIteration = 0;
	} parrotUtils;

	enum ParrotState { SHOOTING_SOUND, RED_LIGHTS };
};

