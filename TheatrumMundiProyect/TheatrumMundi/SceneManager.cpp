#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
#include "Room1.h"
#include "DebugLogRoom.h" //quitar
#include "InitialScene.h"
#include "PipePuzzleScene.h"
#include "ClockPuzzleScene.h"
#include "BooksPuzzleScene.h"
#include "DebugInventoryScene.h"
#include "TeaCupPuzzleScene.h"
#include "checkML.h"
#include "SceneRoomTemplate.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"

//#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
SceneManager::SceneManager()
{
	scenes.resize(SceneName::SCENE_SIZE);

	scenes[SceneName::INITIAL_MENU] = new InitialScene();
	scenes[SceneName::ROOM_1] = new Room1Scene();
	scenes[SceneName::PIPE_PUZZLE] = new PipePuzzleScene();
	scenes[SceneName::CLOCK_PUZZLE] = new ClockPuzzleScene();
	scenes[SceneName::BOOKS_PUZZLE] = new BooksPuzzleScene();
	scenes[SceneName::TEA_CUP_PUZZLE] = new TeaCupPuzzleScene();

	loadScene(SceneName::INITIAL_MENU);
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


