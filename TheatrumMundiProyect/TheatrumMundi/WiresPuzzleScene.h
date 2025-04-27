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
	std::vector<int> winPos = { 3, 1, 5, 2, 4 };

	std::array<Entity*, 5> ports = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> wires = { nullptr, nullptr, nullptr, nullptr, nullptr };
	std::array<Entity*, 5> lights = { nullptr, nullptr, nullptr, nullptr, nullptr };

	int lightsOn;
	bool pressed[5] = { false, false, false, false, false };

	std::vector<int> cableToPort;  // cableToPort[i] = �ndice del puerto al que est� conectado el cable i, -1 si no est� conectado
	std::vector<int> portToCable;  // portToCable[i] = �ndice del cable conectado al puerto i, -1 si libre
	int selectedWireIndex;
};

