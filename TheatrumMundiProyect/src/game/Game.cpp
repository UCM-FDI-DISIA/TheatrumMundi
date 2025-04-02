// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../../TheatrumMundi/Log.h"

#include "../../TheatrumMundi/CSVdataRecolector.h"
#include <Windows.h>


Game* Game::_instance = nullptr;
Game::Game() {
	_exitGame = false;
	
}
Game* Game::Instance()
{
		if (_instance == nullptr)
			_instance = new Game();
		return _instance;
}
Game::~Game() {

	delete _mngr;
	delete _dataManager;
	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

void Game::init() {
	float w ;
	float h  ;
	wscreenScale ;
	hscreenScale ;
//In debug the resolution is const
#ifdef _DEBUG
		w = 1349.0;
		h = 748.0;
		wscreenScale = 1;
		hscreenScale = 1;	
#else
	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);
	wscreenScale = GetSystemMetrics(SM_CXSCREEN) / 1349.0;
	hscreenScale = GetSystemMetrics(SM_CYSCREEN) / 748.0;
#endif

	// initialize the SDL singleton
	if (!SDLUtils::Init("TheatrumMundi",w ,h, "../resources/config/TheatrumMundi.resources.json")) {
		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return;

	}
	sdlutils().hideCursor();

	// Create the manager
	_mngr = new SceneManager();
	_log = new Log();
	
	_dataManager = new DataManager();
	_csvdata = new CSVdataRecolector();
}

void Game::render() const
{
	sdlutils().clearRenderer();
	_mngr->render();

	sdlutils().presentRenderer();
}

void Game::start() {

	// a boolean to exit the loop
	bool _exit = false;

	auto &ihdlr = ih();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!_exitGame) {
		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = sdlutils().regCurrTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			_exitGame = true;
			continue;
		}

		_mngr->update();

		checkCollisions();
		render();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void Game::reset()
{
	//Asserts
	assert(_log != nullptr);
	assert(_dataManager != nullptr);
	assert(_mngr != nullptr);

	//Destroy log and data manager
	Game::Instance()->getLog()->~Log();
	Game::Instance()->getDataManager()->~DataManager();
	Game::Instance()->getSceneManager()->~SceneManager();
	
	//Generate a new Log and a new DataManager
	_log = new Log();
	_dataManager = new DataManager();
	_mngr = new SceneManager();

}

SceneManager* Game::getSceneManager()
{
	assert(_mngr != nullptr);
	return _mngr;
}


void Game::exit()
{
	_exitGame = true;
	
}


DataManager* Game::getDataManager()
{
	return _dataManager;
}

CSVdataRecolector* Game::getCSVDataColector()
{
	return _csvdata;
}
Log* Game::getLog()
{
	return _log;
}

void Game::checkCollisions() {
	
}
