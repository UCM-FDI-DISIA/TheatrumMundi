#pragma once
#include "SceneRoomTemplate.h"
class Room2Scene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;
	bool isOpen;

	//Referencwe to entities which appear and dissapear
	ecs::entity_t organ;
	ecs::entity_t mirror;
	ecs::entity_t secretEntry;
protected:
	enum Room1Event
	{
		InitialDialogue = 0,
		CorpseDialogue,
		TombPuzzleScene,
		TombPuzzleSceneRsv,
		RavenScene,
		RavenSceneRsv,
		DoorScene,
		DoorSceneRsv,
		MosaicPuzzleScene,
		MosaicPuzzleSceneRsv,
		WindowScene,
		Stick,
		OrganPuzzleScene,
		OrganPuzzleSceneRsv,
		Hook,
		SecretExit,
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

