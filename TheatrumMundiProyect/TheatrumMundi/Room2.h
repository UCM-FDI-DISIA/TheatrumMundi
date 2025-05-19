#pragma once
#include "SceneRoomTemplate.h"
#include "RectArea2D.h"
#include "../TheatrumMundi/AudioManager.h"

class ScrollComponent;
class Room2Scene : public SceneRoomTemplate
{
protected:
	enum Room2Event
	{
		InitialDialogue = 0,
		CorpseDialogue,
		TombPuzzleScene,
		TombPuzzleSceneRsv,
		RavenScene,
		RavenSceneRsv,
		DoorScene,
		DoorSceneRsv,
		MosaciZoom,
		MosaicPuzzleScene,
		MosaicPuzzleSceneRsv,
		
		WindowScene,
		WindowSceneResolved,
		Rope,
		OrganZoom,
		OrganPuzzleScene,
		OrganPuzzleSceneRsv,
		Hook,
		SecretEntry,
		ResolveCase,
		ResolveButtons,
		GoodEnd,
		BadEnd,
		event_size,
	};
public:
	Room2Scene();
	~Room2Scene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void unload() override;
private:

	std::string _eventToRead;
	bool isOpen; //Boolean which function is to permite player to enter the mausoleum or not (if the key is on the door or not)
	bool stopAnimation = false;
	bool scrolling = false;
	bool brokenMosaic = false;
	bool MirrorMosaic = false;
	bool isFirstClick = true; //To evitate the insta click in ZoomEntities
	void endDialogue() override;
	void _setRoomEvents();
	void _setRoomAudio();
	void _setGlobalFeatures();

	void _setRoomBackground();
	void _setCaseResolution();
	void _setInteractuables();
	void _setDialog();
	void _setUI();

	struct roomSounds {

		std::shared_ptr<Sound> uiButton;
		std::shared_ptr<Sound> puzzleButton;
		std::shared_ptr<Sound> doorSound;

	} rmSounds;

	struct roomObjects {

		//Entitites without special conditions
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t readyToResolveBotton = nullptr;
		ecs::entity_t blackBackground = nullptr;
		ecs::entity_t logbtn = nullptr;
		ecs::entity_t zoomCorpse = nullptr;
		ecs::entity_t quitButton = nullptr; //Reference to the zoomed Quit Button
		ecs::entity_t zoomOrgan = nullptr; //Image of the zoomed Organ
		ecs::entity_t organ = nullptr; //Image of the organ
		ecs::entity_t rope = nullptr; // Image of the rope
		ecs::entity_t hook = nullptr; //Image of the hook
		ecs::entity_t secretEntryZoom = nullptr; //Image of the ZoomEntry
		ecs::entity_t mosaic = nullptr;
		//ScrollComponent
		ScrollComponent* backgroundScroll = nullptr;

	} rmObjects;
};

