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

	void desconectarCables();


	std::vector<int> actualPos = { -1, -1, -1, -1, -1 };
	std::vector<int> winPos = { 2, 0, 4, 1, 3 };

	std::array<Entity*, 5> ports = { nullptr, nullptr, nullptr, nullptr, nullptr }; //ports entities
	std::array<Entity*, 5> wires = { nullptr, nullptr, nullptr, nullptr, nullptr }; //wires entities
	std::array<Entity*, 5> lights = { nullptr, nullptr, nullptr, nullptr, nullptr }; //lights entities
	std::array<Entity*, 5> offLights = { nullptr, nullptr, nullptr, nullptr, nullptr }; //lights entities

	int lightsOn;

	std::vector<int> cableToPort;  //index of the port connected to cable i, -1 if free
	std::vector<int> portToCable;  //index of the cable connected to port i, -1 if free
	int selectedWireIndex;
	ecs::entity_t gun;

	std::array<Entity*, 5> cablesCortados = { nullptr, nullptr, nullptr, nullptr, nullptr };


	std::array<Entity*, 5> cablesPos0 = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> cablesPos1 = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> cablesPos2 = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> cablesPos3 = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> cablesPos4 = { nullptr, nullptr, nullptr, nullptr, nullptr };



	ecs::entity_t resetButton;
	ecs::entity_t checkButton;
	ecs::entity_t checkButtonOff;

	int timerToCheck;
};

