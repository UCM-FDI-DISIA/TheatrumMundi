#include "ScenePuzzleTemplate.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"

void ScenePuzzleTemplate::Exit()
{
	Game::Instance()->getSceneManager()->popScene();
}

ScenePuzzleTemplate::ScenePuzzleTemplate(): SceneTemplate()
{
}

ScenePuzzleTemplate::~ScenePuzzleTemplate()
{
}
