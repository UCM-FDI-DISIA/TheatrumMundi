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
#include "TutorialScene.h"
#include "TelePuzzleScene.h"
#include "DoorPuzzleScene.h"
#include "XOPuzzleScene.h"
#include "CSVdataRecolector.h"
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
	

}
void SceneManager::init()
{
	loadScene(SceneName::INITIAL_MENU);
}

void SceneManager::popScene()
{
	std::cout << "pop scene" << endl;
	assert(!currentscenes.empty());
	std::string scene = "NONE";
	switch (actsceneindex)
	{
	case SceneName::INITIAL_MENU:
		scene = "INITIAL MENU";
		break;
	case SceneName::ROOM_1:
		scene = "ROOM_1";
		break;
	case SceneName::MIDDLE_ROOM:
		scene = "MIDDLE_ROOM";
		break;
	case SceneName::PIPE_PUZZLE:
		scene = "PIPE_PUZZLE";
		break;
	case SceneName::CLOCK_PUZZLE:
		scene = "CLOCK_PUZZLE";
		break;
	case SceneName::BOOKS_PUZZLE:
		scene = "MIDDLE_ROOM";
		break;
	case -1:
		scene = "TUTORIAL";
		break;
	default:
		break;
	}
	Game::Instance()->getCSVDataColector()->exitScene(scene);

	currentscenes.pop_back();
	int aux;
	for (int i = 0;i < scenes.size();i++){
		if (currentscenes.back() == scenes[i]) aux = i;
	}
	switch (actsceneindex)
	{
	case SceneName::INITIAL_MENU:
		scene = "INITIAL MENU";
		break;
	case SceneName::ROOM_1:
		scene = "ROOM_1";
		break;
	case SceneName::MIDDLE_ROOM:
		scene = "MIDDLE_ROOM";
		break;
	case SceneName::PIPE_PUZZLE:
		scene = "PIPE_PUZZLE";
		break;
	case SceneName::CLOCK_PUZZLE:
		scene = "CLOCK_PUZZLE";
		break;
	case SceneName::BOOKS_PUZZLE:
		scene = "MIDDLE_ROOM";
		break;
	case -1:
		scene = "TUTORIAL";
		break;
	default:
		break;
	}
	Game::Instance()->getCSVDataColector()->enterScene(scene);


}



void SceneManager::loadScene(int index, SceneRoomTemplate* room)
{
	std::string scene = "NONE";
	int aux;
	for (int i = 0;i < scenes.size();i++) {
		if (currentscenes.back() == scenes[i]) aux = i;
	}
	switch (actsceneindex)
	{
	case SceneName::INITIAL_MENU:
		scene = "INITIAL MENU";
		break;
	case SceneName::ROOM_1:
		scene = "ROOM_1";
		break;
	case SceneName::MIDDLE_ROOM:
		scene = "MIDDLE_ROOM";
		break;
	case SceneName::PIPE_PUZZLE:
		scene = "PIPE_PUZZLE";
		break;
	case SceneName::CLOCK_PUZZLE:
		scene = "CLOCK_PUZZLE";
		break;
	case SceneName::BOOKS_PUZZLE:
		scene = "MIDDLE_ROOM";
		break;
	case -1:
		scene = "TUTORIAL";
		break;
	default:
		break;
	}
	Game::Instance()->getCSVDataColector()->exitScene(scene);

	scenes[index]->init(room);
	
	actsceneindex = index;

	switch (actsceneindex)
	{
	case SceneName::INITIAL_MENU:
		scene = "INITIAL MENU";
		break;
	case SceneName::ROOM_1:
		scene = "ROOM_1";
		break;
	case SceneName::MIDDLE_ROOM:
		scene = "MIDDLE_ROOM";
		break;
	case SceneName::PIPE_PUZZLE:
		scene = "PIPE_PUZZLE";
		break;
	case SceneName::CLOCK_PUZZLE:
		scene = "CLOCK_PUZZLE";
		break;
	case SceneName::BOOKS_PUZZLE:
		scene = "MIDDLE_ROOM";
		break;
	case -1:
		scene = "TUTORIAL";
		break;
	default:
		break;
	}
	Game::Instance()->getCSVDataColector()->enterScene(scene);
	currentscenes.push_back(scenes[index]);
	
}

void SceneManager::loadScene(int index)
{
	std::string scene = "NONE";
	if (!currentscenes.empty()) {
		int aux;
		for (int i = 0;i < scenes.size();i++) {
			if (currentscenes.back() == scenes[i]) aux = i;
		}
		switch (actsceneindex)
		{
		case SceneName::INITIAL_MENU:
			scene = "INITIAL MENU";
			break;
		case SceneName::ROOM_1:
			scene = "ROOM_1";
			break;
		case SceneName::MIDDLE_ROOM:
			scene = "MIDDLE_ROOM";
			break;
		case SceneName::PIPE_PUZZLE:
			scene = "PIPE_PUZZLE";
			break;
		case SceneName::CLOCK_PUZZLE:
			scene = "CLOCK_PUZZLE";
			break;
		case SceneName::BOOKS_PUZZLE:
			scene = "MIDDLE_ROOM";
			break;
		case -1:
			scene = "TUTORIAL";
			break;
		default:
			break;
		}
		Game::Instance()->getCSVDataColector()->exitScene(scene);
	}
	scenes[index]->init();
	
	actsceneindex = index;

	switch (actsceneindex)
	{
	case SceneName::INITIAL_MENU:
		scene = "INITIAL MENU";
		break;
	case SceneName::ROOM_1:
		scene = "ROOM_1";
		break;
	case SceneName::MIDDLE_ROOM:
		scene = "MIDDLE_ROOM";
		break;
	case SceneName::PIPE_PUZZLE:
		scene = "PIPE_PUZZLE";
		break;
	case SceneName::CLOCK_PUZZLE:
		scene = "CLOCK_PUZZLE";
		break;
	case SceneName::BOOKS_PUZZLE:
		scene = "MIDDLE_ROOM";
		break;
	case -1:
		scene = "TUTORIAL";
		break;
	default:
		break;
	}
	Game::Instance()->getCSVDataColector()->enterScene(scene);
	currentscenes.push_back(scenes[index]);
}

void SceneManager::unloadScene()
{
	currentscenes.back()->unload();
	currentscenes.pop_back();
	refresh();
}
void SceneManager::render() {
	currentscenes.back()->render();
}
void SceneManager::refresh()
{
	currentscenes.back()->refresh();
}

void SceneManager::update()
{
	currentscenes.back()->update();
}

int SceneManager::getSceneIndex()
{
	return actsceneindex+1;
}



SceneManager::~SceneManager()
{
	for (auto a : scenes) 
		if(a!=nullptr) delete a;

	currentscenes.clear();
}

void SceneManager::ResolveActScene()
{
}



void SceneManager::ResetSceneManager()
{

	scenes.resize(SceneName::SCENE_SIZE);

	scenes[SceneName::INITIAL_MENU] = new InitialScene();
	scenes[SceneName::MIDDLE_ROOM] = new MiddleRoomScene();
	scenes[SceneName::ROOM_1] = new Room1Scene();
	scenes[SceneName::PIPE_PUZZLE] = new PipePuzzleScene();
	scenes[SceneName::CLOCK_PUZZLE] = new ClockPuzzleScene();
	scenes[SceneName::BOOKS_PUZZLE] = new BooksPuzzleScene();
	scenes[SceneName::TEA_CUP_PUZZLE] = new TeaCupPuzzleScene();
	scenes[SceneName::MUSIC_PUZZLE] = new MusicPuzzleScene();
	scenes[SceneName::MOSAIC_SCENE] = new MosaicPuzzleScene();
	scenes[SceneName::DRAG_PUZZLE] = new DragPuzzleScene();
	scenes[SceneName::TUTORIAL_SCENE] = new TutorialScene();
	scenes[SceneName::XO_PUZZLE] = new XOPuzzleScene();
	scenes[SceneName::TELE_PUZZLE] = new TelePuzzleScene();

	loadScene(SceneName::INITIAL_MENU);

}


