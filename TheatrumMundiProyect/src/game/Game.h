// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"
#include "../../TheatrumMundi/DataManager.h"

class Transform;
class GameState;
class CSVdataRecolector;
class Game {
	
private:
	
	 SceneManager* _mngr;
	 
	 DataManager* _datamngr;
	 CSVdataRecolector* _csvdata;
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

