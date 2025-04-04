#pragma once
#include "SceneRoomTemplate.h"

class TutorialScene : public SceneRoomTemplate
{
private:
	std::string _eventToRead;

	ecs::entity_t botonBack;
protected:
	enum TutorialEvent
	{
		Dialog0 = 0,
		Dialog1,
		Dialog2,
		Dialog3,
		Dialog4,
		Dialog5,
		Dialog6,
		Dialog7,
		Dialog8,
		ClockPuzzleSnc,
		ClockPuzzleRsv,
		TeleScene,
		Spoon,
		ResolveCase,
		ResolveBottons,
		MobileDialogue,
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

	void setInteractable(Entity* e);
};

