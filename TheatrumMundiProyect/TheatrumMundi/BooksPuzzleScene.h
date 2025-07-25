#pragma once
#include "ScenePuzzleTemplate.h"

class array;
class vector;
class SceneRoomTemplate;


class BooksPuzzleScene : public ScenePuzzleTemplate
{
	friend class Room1Scene;
public:
	BooksPuzzleScene(/*Room1Scene* room1*/);
	~BooksPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override; //HERE WE PUT THE CORRECT COMBINATION
	void Win() override;
	void ResolveScene() override;
private:
	//std::array<int, 10> comb;
	//std::array<int, 3> myComb;

	//Room1Scene* room;
	
	std::vector<int> comb;
	std::vector<int> myComb;

	ecs::entity_t ButtonBookFirst;
	ecs::entity_t ButtonBookSecond;
	ecs::entity_t ButtonBookThird;
};

