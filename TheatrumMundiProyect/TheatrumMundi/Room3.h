#pragma once
#include "SceneRoomTemplate.h"
#include "RectArea2D.h"
#include "../TheatrumMundi/AudioManager.h"

#include <SDL.h>

class ScrollComponent;
class Room3Scene : public SceneRoomTemplate {
protected:
	enum Room3Event {
		InitialDialogue = 0,
		CorpseDialogue,
		LightsOn,
		LightsOff,
		LightsRed,
		WiresPuzzleScene,
		WiresSceneRsv,
		CircleLockPuzzleScene,
		CircleLockSceneRsv,
		BalancePuzzleScene,
		BalanceSceneRsv,
		ParrotScene,
		ParrotSceneRsv,
		MorseCodePuzzleScene,
		MorseCodeSceneRsv,
		ZoomMorseGuide,
		ZoomRadio,
		ResolveCase,
		ResolveButtons,
		GoodEnd,
		BadEnd,
		event_size,
	};

public:

	enum Phase {
		LIGHTS_OFF,
		LIGHTS_ON,
		LIGHTS_RED
	};

	Room3Scene();
	~Room3Scene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void unload() override;
private:
	std::string _eventToRead;
	bool stopAnimation = false;
	//Determined by the integer PhaseRoom3 from DataManager
	//bool LightsToOn = false; //To interact with determined entities when the room is with light
	//bool LightsToRed = false; //To interact with determined entities when de room is with red light
	bool scrolling = false;

	void endDialogue() override;
	void _setRoomEvents();
	void _setRoomAudio();
	void _setGlobalFeatures();

	void _setRoomBackground();
	void _setCaseResolution();
	void _setInteractuables();
	void _setDialog();
	void _setUI();

	void _resetSounds();

	struct roomSounds {
		std::shared_ptr<Sound> uiButton;
		std::shared_ptr<Sound> puzzleButton;
		std::shared_ptr<Sound> doorSound;
		std::shared_ptr<Sound> shootSound;
		std::shared_ptr<Sound> explosionSound;
		std::shared_ptr<Sound> morse_Sound;
		std::shared_ptr<Sound> morse_Sound_Low;
		std::shared_ptr<Sound> s_Sound;
		std::shared_ptr<Sound> t_Sound;
		std::shared_ptr<Sound> o_Sound;
		std::shared_ptr<Sound> p_Sound;

	} rmSounds;

	struct roomObjects {
		//Entitites without special conditions
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t readyToResolveBotton = nullptr;
		ecs::entity_t blackBackground = nullptr;
		ecs::entity_t logbtn = nullptr;
		ecs::entity_t zoomCorpse = nullptr;
		ecs::entity_t zoomMorseGuide = nullptr;
		ecs::entity_t feather = nullptr;
		ecs::entity_t quitButton = nullptr; //Reference to the zoomed Quit Button
		
		ecs::entity_t cablesPuzzle = nullptr; //Image of the cablesPuzzle
		ecs::entity_t boxOfficeMorseCodeB = nullptr; //Image of the first boxOffice
		ecs::entity_t boxOfficeCircleLockP = nullptr; //Image of the second boxOffice
		ecs::entity_t radio = nullptr; //Image of the radio
		ecs::entity_t zoomRadio = nullptr;
		
		ecs::entity_t parrot = nullptr; //Image of the parrot
		ecs::entity_t balance = nullptr; //Image of the balance
		ecs::entity_t locker = nullptr;
		ecs::entity_t safeBox = nullptr; //Image of the safeBox

		////Entities with special conditions (only appear with one specific event)
		ecs::entity_t backgroundLeftIllumination = nullptr;
		ecs::entity_t backgroundRightIllumination = nullptr;
		//std::pair<ecs::entity_t, bool>organMosaic; //Image of the entry to the organ room
		//std::pair<ecs::entity_t, bool> mirror; //Image which reflects information
		//std::pair<ecs::entity_t, bool> secretEntry; //Image reflected

		//ScrollComponent
		ScrollComponent* backgroundScroll = nullptr;
	} rmObjects;

	struct ParrotUtils{
		Uint32 lastSoundTime = 0; // The time where the parrot emited the last noise
		std::vector<std::shared_ptr<Sound>> codeSequenceSounds; // Sequence of sounds in the second phase
		int codeSeqIteration = 0;
		bool zoomParrotRadio = false; //We use this for the volume of the zoomIn in the radio
	} parrotUtils;
};