#pragma once
#include "SceneRoomTemplate.h"

class TutorialScene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;

	ecs::entity_t ChangeRoom1;

	ecs::entity_t doorImage;

	ecs::entity_t passwordButton;

	ecs::entity_t botonBack;

	ecs::entity_t television;

	ecs::entity_t antenna;

	ecs::entity_t inventoryButton;

	int dialogCount;
protected:
	enum TutorialEvent
	{
		Dialog0 = 0,
		Dialog1,
		Dialog2,
		Dialog3,
		
		Dialog5,
		Dialog6,
		
		TeleScene,
		Antenna,
		
		LOGENABLE,
		LOGDESABLE,
		event_size,
	};
public:
	TutorialScene();
	~TutorialScene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;
	void endDialogue() override;
	void closedLog() override;

};

