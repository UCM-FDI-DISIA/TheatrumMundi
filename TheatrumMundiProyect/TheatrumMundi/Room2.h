#pragma once
#include "SceneRoomTemplate.h"
class Room2Scene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;
	bool isOpen; //Boolean which function is to permite player to enter the mausoleum or not (if the key is on the door or not)

	//Reference to entities which appear and dissapear
	ecs::entity_t _quitButton; //Reference to the zoomed Quit Button
	ecs::entity_t zoomCorpse; //Image of the zoomed Corpse
	ecs::entity_t organMosaic; //Image of the entry to the organ room
	ecs::entity_t zoomOrgan; //Image of the zoomed Organ
	ecs::entity_t organ; //Image of the organ
	ecs::entity_t rope; // Image of the rope
	ecs::entity_t hook; //Image of the hook
	ecs::entity_t mirror; //Image which reflects information
	ecs::entity_t secretEntry; //Image reflected
	ecs::entity_t secretEntryZoom; //Image of the ZoomEntry

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
	void refresh() override;
	void unload() override;
};

