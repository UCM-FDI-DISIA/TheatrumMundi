#pragma once
#include <list>
#include<vector>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"

class SceneTemplate;
class SceneRoomTemplate;
using namespace ecs;
enum SceneName {
	INITIAL_MENU = 0,
	MIDDLE_ROOM,
	ROOM_1,
	PIPE_PUZZLE,
	BOOKS_PUZZLE,
	CLOCK_PUZZLE,
	TEA_CUP_PUZZLE,
	ROOM_2,
	MUSIC_PUZZLE,
	RAVEN_SCENE,
	DOOR_SCENE,
	MOSAIC_SCENE,
	WINDOW_SCENE,
	TOMB_SCENE,
	DRAG_PUZZLE,
	XO_PUZZLE,
	ROOM_3,
	LOCKER_PUZZLE,
	TUTORIAL_SCENE,
	TELE_PUZZLE,
	CREDITS,
	WIRES_PUZZLE,
	BALANCE_PUZZLE,
	BOX,
	SCENE_SIZE
};
class SceneManager
{
private:
	std::vector<SceneTemplate*> scenes;
	std::list<SceneTemplate*> currentscenes;
	int actsceneindex;
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
	 void refresh();
	 //update of the currentscene
	 void update();
	 //Get the actSceneIndex ONLY USE FOR PLAYTESTING
	 int getSceneIndex(); 
	 //delete of enitities
	~SceneManager();
	void ResolveActScene();
	void init();
	void ResetSceneManager();
};

