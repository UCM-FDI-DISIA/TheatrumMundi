#pragma once
#include <list>
#include<vector>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"

class SceneTemplate;
class SceneRoomTemplate;
using namespace ecs;
enum SceneName {
	INITIAL_MENU = 0,
	MIDDLE_ROOM_LOG,
	MIDDLE_ROOM,
	ROOM_1,
	PIPE_PUZZLE,
	BOOKS_PUZZLE,
	CLOCK_PUZZLE,
	TEA_CUP_PUZZLE,
	SCENE_SIZE
};
class SceneManager
{
private:
	std::vector<SceneTemplate*> scenes;
	std::list<SceneTemplate*> currentscenes;
protected:
public:
	SceneManager();
	 void popScene();
	 void loadScene(int index, SceneRoomTemplate* room);
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

