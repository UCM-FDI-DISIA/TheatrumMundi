// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"
#include "../../TheatrumMundi/DialogueManager.h"

class Transform;
class GameState;

class Game {
	
private:
	
	 SceneManager* _mngr;
	 DialogueManager* _Dmngr;
	 Game();
	
	 void checkCollisions();
public:
	static Game* Instance();
	~Game();
	void init();
	void start();
	SceneManager* getSceneManager();
	DialogueManager* getDialogueManager();
	Game& operator=(const Game& o) = delete;
	Game(const Game& o) = delete;
private:
	static Game* _instance;
};

