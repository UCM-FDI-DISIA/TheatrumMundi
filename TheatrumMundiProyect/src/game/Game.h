// This file is part of the course TPV2@UCM - Samir Genaim
#pragma once

#include <vector>
#include "../../TheatrumMundi/SceneManager.h"
#include "../../TheatrumMundi/DataManager.h"
#include "../../TheatrumMundi/ReadDialog.h"

class Transform;
class GameState;
class Log;

class CSVdataRecolector;
class Game {
	
private:
	
	 SceneManager* _mngr;
	 Log* _log;
	 ReadDialog* dialogueReader;
	 DataManager* _dataManager;
	 CSVdataRecolector* _csvdata;
	 Game();
	
	 void checkCollisions();
public:
	
	static Game* Instance();
	~Game();
	void init();
	void render()const;
	void start();
	void reset(); //Reset the progress of the game and creates one new
	SceneManager* getSceneManager();
	DataManager* getDataManager();
	CSVdataRecolector* getCSVDataColector();
	ReadDialog* getReadDialogue();
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

