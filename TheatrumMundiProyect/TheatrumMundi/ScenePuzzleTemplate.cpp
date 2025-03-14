#include "ScenePuzzleTemplate.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"


void ScenePuzzleTemplate::Exit()
{
	Game::Instance()->getSceneManager()->popScene();
}


/// <summary>
/// Searcch if the item is already created
/// </summary>
/// <param name="id"></param> --> item to search
/// <returns></returns> --> return true if the item exists and false in the other case
bool ScenePuzzleTemplate::ItemAlreadyCreated(const std::string& id)
{
	for (auto a : invID) {
		if (a == id) return true;
	}
	return false;
}

ScenePuzzleTemplate::ScenePuzzleTemplate(): SceneTemplate()
{

}

ScenePuzzleTemplate::~ScenePuzzleTemplate()
{
	//if memory leak delete the vector of entities
}
