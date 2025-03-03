#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
#include "Room1.h"
#include "DebugLogRoom.h"
#include "InitialScene.h"
#include "PipePuzzleScene.h"
#include "ClockPuzzleScene.h"
#include "BooksPuzzleScene.h"
#include "checkML.h"
#include "SceneRoomTemplate.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"

//#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
SceneManager::SceneManager()
{
	scenes.resize(sceneName::Scene_Size);
	scenes[initialMenu] = new InitialScene();
	scenes[Room1] = new Room1Scene();
	scenes[PipePuzzle] = new PipePuzzleScene();
	scenes[ClockPuzzle] = new ClockPuzzleScene();
	scenes[BooksPuzzle] = new BooksPuzzleScene();

	loadScene(initialMenu);
	
}

void SceneManager::popScene()
{
		assert(currentscenes.empty());
		currentscenes.pop_back();
}



void SceneManager::loadScene(int index, SceneRoomTemplate* room)
{
	scenes[index]->init(room);
	currentscenes.push_back(scenes[index]);
	
	
}

void SceneManager::loadScene(int index)
{
	scenes[index]->init();
	currentscenes.push_back(scenes[index]);
}

void SceneManager::unloadScene()
{
	currentscenes.back()->unload();
	currentscenes.pop_back();
	refrest();
}
void SceneManager::render() {
	currentscenes.back()->render();
}
void SceneManager::refrest()
{
	currentscenes.back()->refresh();
}

void SceneManager::update()
{
	currentscenes.back()->update();
}



SceneManager::~SceneManager()
{
	for (auto a : scenes) delete a;

	currentscenes.clear();
}


