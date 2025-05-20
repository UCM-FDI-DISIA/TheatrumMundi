#pragma once
#include "ScenePuzzleTemplate.h"
class Image;

class Box : public ScenePuzzleTemplate
{
public:
	Box();
	~Box();
	void init(SceneRoomTemplate* sr) override;
	void pushButton(int i);
	void Win();
	
private:

	std::vector<Image*> buttonImages;
	std::vector<std::string> buttonImagesIni; 
	ecs::entity_t knife;
	ecs::entity_t flashlight;
	int stage = 0;
	bool lastWright = false;
	bool completed = false;

	struct roomObjects {
		ecs::entity_t background = nullptr;
		ecs::entity_t quitButton = nullptr;
		ecs::entity_t inventoryButton = nullptr;
		ecs::entity_t logbtn = nullptr;
		ecs::entity_t bulletsEntity = nullptr;
	} rmObjects;
};

