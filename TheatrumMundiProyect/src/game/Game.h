// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"

class Transform;
class GameState;
class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	void checkCollisions();
	 SceneManager* _mngr;
	
};

