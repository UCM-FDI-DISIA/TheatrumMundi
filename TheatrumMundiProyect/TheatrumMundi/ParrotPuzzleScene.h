#pragma once
#include "ScenePuzzleTemplate.h"
#include <SDL.h>

class Sound;
class BehaviorStateComponent;

class ParrotPuzzleScene : public ScenePuzzleTemplate
{
public:
	ParrotPuzzleScene();
	~ParrotPuzzleScene();

	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override;
	void Win() override;
	void ResolveScene() override;

private:

	void _setGlobalFeatures();
	void _setRoomAudio();
	void _setRoomBackground();
	void _setInteractuables(SceneRoomTemplate* sr);
	void _setDialog();
	void _setUI();
	bool isItemHand(const std::string& itemId) override;

	ecs::entity_t _backButton;


	BehaviorStateComponent* parrotStateCom;

	struct roomObjects {
		ecs::entity_t background = nullptr;
		ecs::entity_t quitButton = nullptr;
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t logbtn = nullptr;
		ecs::entity_t bulletsEntity = nullptr;
	} rmObjects;

	struct roomSounds {
		std::shared_ptr<Sound> uiButton;
		std::shared_ptr<Sound> puzzleButton;
		std::shared_ptr<Sound> doorSound;
		std::shared_ptr<Sound> shootSound;
		std::shared_ptr<Sound> explosionSound;
		std::shared_ptr<Sound> morse_Sound;
		std::shared_ptr<Sound> s_Sound;
		std::shared_ptr<Sound> t_Sound;
		std::shared_ptr<Sound> o_Sound;
		std::shared_ptr<Sound> p_Sound;

	} rmSounds;

	struct ParrotUtils {
		ecs::entity_t parrotEnt = nullptr;
		Uint32 lastSoundTime = 0; // The time where the parrot emited the last noise
		std::vector<std::shared_ptr<Sound>> codeSequenceSounds; // Sequence of sounds in the second phase
		int codeSeqIteration = 0;
	} parrotUtils;

	enum ParrotState { DEATH = 3 };
};

