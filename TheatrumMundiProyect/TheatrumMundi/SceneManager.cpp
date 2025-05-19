#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"
#include "Room1.h"
#include "Room2.h"
#include "Room3.h"
#include "roomprueba.h"
#include "InitialScene.h"
#include "PipePuzzleScene.h"
#include "ClockPuzzleScene.h"
#include "BooksPuzzleScene.h"
#include "TeaCupPuzzleScene.h"
#include "checkML.h"
#include "SceneRoomTemplate.h"
#include "MosaicPuzzleScene.h"
#include "RavenPuzzleScene.h"
#include "WindowPuzzleScene.h"
#include "MiddleRoomScene.h"
#include "MusicPuzzleScene.h"
#include "TombPuzzleScene.h"
#include "DragPuzzleScene.h"
#include "TutorialScene.h"
#include "TelePuzzleScene.h"
#include "DoorPuzzleScene.h"
#include "XOPuzzleScene.h"
#include "BalancePuzzleScene.h"
#include "CreditsScene.h"
#include "WiresPuzzleScene.h"
#include "LockerPuzzle.h"
#include "CSVdataRecolector.h"
#include "BalancePuzzleScene.h"
#include "Box.h"
#include "ParrotPuzzleScene.h"

SceneManager::SceneManager()
{
	//If this content is edited, please put the same code in ResetSceneManager
	scenes.resize(SceneName::SCENE_SIZE);
	auto tombScene = new TombPuzzleScene(); //For the chain Puzzle, to take the reference

	scenes[SceneName::INITIAL_MENU] = new InitialScene();
	scenes[SceneName::MIDDLE_ROOM] = new MiddleRoomScene();
	scenes[SceneName::ROOM_1] = new Room1Scene();
	scenes[SceneName::ROOM_2] = new Room2Scene();
	scenes[SceneName::ROOM_3] = new Room3Scene();
	scenes[SceneName::BALANCE_PUZZLE] = new BalancePuzzleScene();
	scenes[SceneName::WIRES_PUZZLE] = new WiresPuzzleScene();
	scenes[SceneName::LOCKER_PUZZLE] = new LockerPuzzle();
	scenes[SceneName::PIPE_PUZZLE] = new PipePuzzleScene();
	scenes[SceneName::CLOCK_PUZZLE] = new ClockPuzzleScene();
	scenes[SceneName::BOOKS_PUZZLE] = new BooksPuzzleScene();
	scenes[SceneName::TEA_CUP_PUZZLE] = new TeaCupPuzzleScene();
	scenes[SceneName::MUSIC_PUZZLE] = new MusicPuzzleScene();
	scenes[SceneName::RAVEN_SCENE] = new RavenPuzzleScene();
	scenes[SceneName::DOOR_SCENE] = new DoorPuzzleScene();
	scenes[SceneName::WINDOW_SCENE] = new WindowPuzzleScene();
	scenes[SceneName::MOSAIC_SCENE] = new MosaicPuzzleScene();
	scenes[SceneName::TOMB_SCENE] = tombScene;
	scenes[SceneName::DRAG_PUZZLE] = new DragPuzzleScene(tombScene);
	scenes[SceneName::XO_PUZZLE] = new XOPuzzleScene(tombScene);
	scenes[SceneName::TUTORIAL_SCENE] = new TutorialScene();
	scenes[SceneName::TELE_PUZZLE] = new TelePuzzleScene();
	scenes[SceneName::CREDITS] = new CreditsScene();
	scenes[SceneName::BOX] = new Box();
	scenes[SceneName::PARROT_PUZZLE] = new ParrotPuzzleScene();
}
void SceneManager::init()
{
	loadScene(SceneName::INITIAL_MENU);
}

void SceneManager::popScene()
{
	assert(!currentscenes.empty());
	currentscenes.pop_back();
	currentscenes.back()->closedMenus();
	if(currentscenes.back() == scenes[SceneName::MIDDLE_ROOM]) currentscenes.back()->init();

}



void SceneManager::loadScene(int index, SceneRoomTemplate* room)
{
	
	//loadResouces
// #ifndef //  _LOADALLRESOURCES
	
	switch (index)
	{
	case INITIAL_MENU:
		sdlutils().ClearMaps();
		sdlutils().loadReasources("../resources/config/TheatrumMundiInitialMenu.resources.json");
		break;
	case TUTORIAL_SCENE:
		sdlutils().ClearMaps();
		sdlutils().loadReasources("../resources/config/TheatrumMundiTutorial.resources.json");
		break;
	case ROOM1:
		sdlutils().ClearMaps();
		sdlutils().loadReasources("../resources/config/TheatrumMundiRoom1.resources.json");
		break;
	default:
		break;
	}
// #endif // !_LOADALLRESOURCES
	scenes[index]->init(room);
	actsceneindex = index;
	currentscenes.push_back(scenes[index]);
	currentscenes.back()->closedMenus();
	
}

void SceneManager::loadScene(int index)
{
	//loadResouces
#ifndef _LOADALLRESOURCES
	
	switch (index)
	{
	case INITIAL_MENU:
		sdlutils().ClearMaps();
		sdlutils().loadReasources("../resources/config/TheatrumMundiInitialMenu.resources.json");
		break;
	case TUTORIAL_SCENE:
		sdlutils().ClearMaps();
		sdlutils().loadReasources("../resources/config/TheatrumMundiTutorial.resources.json");
		break;
	case ROOM_1:
		sdlutils().ClearMaps();

		sdlutils().loadReasources("../resources/config/TheatrumMundiRoom1.resources.json");
		break;
	case ROOM_2:
		sdlutils().ClearMaps();

		sdlutils().loadReasources("../resources/config/TheatrumMundiRoom2.resources.json");
		break;
	case ROOM_3:
		sdlutils().ClearMaps();

		sdlutils().loadReasources("../resources/config/TheatrumMundiRoom3.resources.json");
		break;
	default:
		break;
	}
#endif // !_LOADALLRESOURCES

	scenes[index]->init();
	actsceneindex = index;
	currentscenes.push_back(scenes[index]);
	currentscenes.back()->closedMenus();
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
	for (int i = 0; i < SCENE_SIZE;i++) {
		
		 delete scenes[i];

	}
	scenes.resize(SceneName::SCENE_SIZE);
	currentscenes.clear();
	auto tombScene = new TombPuzzleScene(); //For the chain Puzzle, to take the reference

	scenes[SceneName::INITIAL_MENU] = new InitialScene();
	scenes[SceneName::MIDDLE_ROOM] = new MiddleRoomScene();
	scenes[SceneName::ROOM_1] = new Room1Scene();
	scenes[SceneName::ROOM_2] = new Room2Scene();
	scenes[SceneName::ROOM_3] = new Room3Scene();
	scenes[SceneName::BALANCE_PUZZLE] = new BalancePuzzleScene();
	scenes[SceneName::WIRES_PUZZLE] = new WiresPuzzleScene();
	scenes[SceneName::LOCKER_PUZZLE] = new LockerPuzzle();
	scenes[SceneName::PIPE_PUZZLE] = new PipePuzzleScene();
	scenes[SceneName::CLOCK_PUZZLE] = new ClockPuzzleScene();
	scenes[SceneName::BOOKS_PUZZLE] = new BooksPuzzleScene();
	scenes[SceneName::TEA_CUP_PUZZLE] = new TeaCupPuzzleScene();
	scenes[SceneName::MUSIC_PUZZLE] = new MusicPuzzleScene();
	scenes[SceneName::RAVEN_SCENE] = new RavenPuzzleScene();
	scenes[SceneName::DOOR_SCENE] = new DoorPuzzleScene();
	scenes[SceneName::WINDOW_SCENE] = new WindowPuzzleScene();
	scenes[SceneName::MOSAIC_SCENE] = new MosaicPuzzleScene();
	scenes[SceneName::TOMB_SCENE] = tombScene;
	scenes[SceneName::DRAG_PUZZLE] = new DragPuzzleScene(tombScene);
	scenes[SceneName::XO_PUZZLE] = new XOPuzzleScene(tombScene);
	scenes[SceneName::TUTORIAL_SCENE] = new TutorialScene();
	scenes[SceneName::TELE_PUZZLE] = new TelePuzzleScene();
	scenes[SceneName::CREDITS] = new CreditsScene();
	scenes[SceneName::BOX] = new Box();
	scenes[SceneName::PARROT_PUZZLE] = new ParrotPuzzleScene();
	loadScene(SceneName::INITIAL_MENU);
}




