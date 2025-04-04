#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
#include "Room1.h"
#include "Room2.h"
#include "InitialScene.h"
#include "PipePuzzleScene.h"
#include "ClockPuzzleScene.h"
#include "BooksPuzzleScene.h"
#include "TeaCupPuzzleScene.h"
#include "checkML.h"
#include "SceneRoomTemplate.h"
#include"DebugInventoryScene.h"
#include "MosaicPuzzleScene.h"
#include "RavenPuzzleScene.h"
#include "MiddleRoomScene.h"
#include "MusicPuzzleScene.h"
#include "DragPuzzleScene.h"
#include "DoorPuzzleScene.h"
#include "XOPuzzleScene.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"
SceneManager::SceneManager()
{
	scenes.resize(SceneName::SCENE_SIZE);

	scenes[SceneName::INITIAL_MENU] = new InitialScene();
	scenes[SceneName::MIDDLE_ROOM] = new MiddleRoomScene();
	scenes[SceneName::ROOM_1] = new Room1Scene();
	scenes[SceneName::ROOM_2] = new Room2Scene();
	scenes[SceneName::PIPE_PUZZLE] = new PipePuzzleScene();
	scenes[SceneName::CLOCK_PUZZLE] = new ClockPuzzleScene();
	scenes[SceneName::BOOKS_PUZZLE] = new BooksPuzzleScene();
	scenes[SceneName::TEA_CUP_PUZZLE] = new TeaCupPuzzleScene();
	scenes[SceneName::MUSIC_PUZZLE] = new MusicPuzzleScene();
	scenes[SceneName::RAVEN_SCENE] = new RavenPuzzleScene();
	scenes[SceneName::DOOR_SCENE] = new DoorPuzzleScene();
	scenes[SceneName::MOSAIC_SCENE] = new MosaicPuzzleScene();
	scenes[SceneName::DRAG_PUZZLE] = new DragPuzzleScene();
	scenes[SceneName::XO_PUZZLE] = new XOPuzzleScene();
	loadScene(SceneName::ROOM_2);
}

void SceneManager::popScene()
{
	std::cout << "pop scene" << endl;
	assert(!currentscenes.empty());
	currentscenes.pop_back();
}



void SceneManager::loadScene(int index, SceneRoomTemplate* room)
{
	scenes[index]->init(room);
	actsceneindex = index;
	currentscenes.push_back(scenes[index]);
	
}

void SceneManager::loadScene(int index)
{
	scenes[index]->init();
	actsceneindex = index;
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
	for (auto a : scenes) 
		if(a!=nullptr) delete a;

	currentscenes.clear();
}


