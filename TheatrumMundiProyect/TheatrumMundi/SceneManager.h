#pragma once
#include <list>
#include<vector>
//#include "../../TheatrumMundiProyect/src/ecs/Entity.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
class SceneTemplate;
//class Entity;

using namespace std;
class SceneManager
{
private:
	std::vector<SceneTemplate*> scenes;
	std::list<SceneTemplate*> currentscenes;
protected:
public:
	SceneManager();
	 void popScene();
	 void loadScene(int index, const ecs::entity_t& extradata);
	 //pushback
	 void loadScene(int index);
	 //pop of act scene
	 void unloadScene();

	 void update();
	 //delete of enitities
	~SceneManager();

};

