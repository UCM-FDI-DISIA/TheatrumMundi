#pragma once
#include "ScenePuzzleTemplate.h"
#include <vector>

class array;
class vector;
class SceneRoomTemplate;

class WiresPuzzleScene : public ScenePuzzleTemplate
{
public:
	WiresPuzzleScene(/*Room1Scene* room1*/);
	~WiresPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	void unload() override;
	bool Check() override; //HERE WE PUT THE CORRECT COMBINATION
	void Win() override;
	void ResolveScene() override;

private:
	ecs::entity_t logbtn;
	std::vector<int> actualPos = { -1, -1, -1, -1, -1 };
	std::vector<int> winPos = { 2, 0, 4, 1, 3 };

	std::array<Entity*, 5> ports = { nullptr, nullptr, nullptr, nullptr, nullptr }; //ports entities
	std::array<Entity*, 5> wires = { nullptr, nullptr, nullptr, nullptr, nullptr }; //wires entities
	std::array<Entity*, 5> lights = { nullptr, nullptr, nullptr, nullptr, nullptr }; //lights entities

	int lightsOn;
	bool pressed[5] = { false, false, false, false, false };

	std::vector<int> cableToPort;  //index of the port connected to cable i, -1 if free
	std::vector<int> portToCable;  //index of the cable connected to port i, -1 if free
	int selectedWireIndex;
};

