// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"

class Transform;
class GameState;

class Game {
	
private:
	
	 SceneManager* _mngr;
	
	 Game();
	
	 void checkCollisions();
public:
	static Game* Instance();
	virtual ~Game();
	void init();
	void start();
	SceneManager* getSceneManager();
	Game& operator=(const Game& o) = delete;
	Game(const Game& o) = delete;
	void exit();
private:
	static Game* _instance;
	bool _exitGame;
};

