#include "SceneManager.h"
#include <assert.h>
#include "SceneTemplate.h"
SceneManager::SceneManager()
{

}

void SceneManager::popScene()
{
		assert(currentscenes.empty());
		currentscenes.pop_back();
}

void SceneManager::loadScene(int index, const ecs::entity_t& extradata)
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
}

void SceneManager::update()
{
	currentscenes.back()->update();
}



SceneManager::~SceneManager()
{
	for (auto a : scenes) delete a;
	for (auto a : currentscenes) delete a;
}
