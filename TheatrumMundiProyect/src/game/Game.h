// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"
#include "../../TheatrumMundi/DataManager.h"

class Transform;
class GameState;
class Log;

class Game {
	
private:
	
	 SceneManager* _mngr;
	 Log* _log;
	 
	 DataManager* _datamngr;
	 Game();
	
	 void checkCollisions();
public:
	
	static Game* Instance();
	~Game();
	void init();
	void render()const;
	void start();
	SceneManager* getSceneManager();
	DataManager* getDataManager();
	Log* getLog();
	Game& operator=(const Game& o) = delete;
	Game(const Game& o) = delete;
	void exit();
	 float wscreenScale;
	 float hscreenScale;
private:
	static Game* _instance;
	bool _exitGame;
protected:
	
};

