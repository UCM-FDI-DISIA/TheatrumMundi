#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
#include "Room1.h"
#include "DebugLogRoom.h"
#include "InitialScene.h"
#include "PipePuzzleScene.h"
#include "BooksPuzzleScene.h"
#include "CalendearMobileTestScene.h"
#include "checkML.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"

//#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
SceneManager::SceneManager()
{
scenes.push_back(new CalendearMobileTestScene());
	//scenes.push_back(new PipePuzzleScene());
//	scenes.push_back(new InitialScene());
	//scenes.push_back(new Room1());
	//scenes.push_back(new DebugLogRoom());
  
	
	//scenes.push_back(new BooksPuzzleScene());
	//scenes.push_back(new DebugLogRoom());
	//scenes.push_back(new Room1());
	loadScene(0);
	
}

void SceneManager::popScene()
{
		assert(currentscenes.empty());
		currentscenes.pop_back();
}

void SceneManager::loadScene(int index, ecs::entity_t& extradata)
{
	scenes[index]->init(extradata);
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


