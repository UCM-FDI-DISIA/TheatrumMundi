#pragma once
#include <list>
#include<vector>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"

class SceneTemplate;

using namespace ecs;
class SceneManager
{
private:
	std::vector<SceneTemplate*> scenes;
	std::list<SceneTemplate*> currentscenes;
protected:
public:
	SceneManager();
	 void popScene();
	 void loadScene(int index, entity_t& extradata);
	 //pushback
	 void loadScene(int index);
	 //unload of the ram de currentscene and pop it
	 void unloadScene();
	 void render();
	 //refrest the currect scene 
	 void refrest();
	 //update of the currentscene
	 void update();
	 //delete of enitities
	~SceneManager();

};

