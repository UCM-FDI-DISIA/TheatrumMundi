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

	struct ParrotUtils {
		Uint32 lastSoundTime = 0; // The time where the parrot emited the last noise
		std::vector<std::shared_ptr<Sound>> codeSequenceSounds; // Sequence of sounds in the second phase
		int codeSeqIteration = 0;
	} parrotUtils;

	enum ParrotState { SHOOTING_SOUND, RED_LIGHTS };
};

