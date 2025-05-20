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
	loadedResources = -1;
}
Game* Game::Instance()
{
		if (_instance == nullptr)
			_instance = new Game();
		return _instance;
}
Game::~Game() {
	
	delete _mngr;
	delete dialogueReader;
	delete _dataManager;
	delete _log;
	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance()) {
		SDLUtils::Release();
	}
	if (AudioManager::HasInstance()) {
		AudioManager::Release();
	}

}

void Game::init() {
	
	
	float w;
	float h;
	wscreenScale;
	hscreenScale;
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

	if (!AudioManager::Init()) {
		std::cerr << "Failed to initialize fmod." << std::endl;
		return;
	}

	//Load all thee game resources in the global json else the resources have been loaded in scenes 
	// initialize the SDL singleton
	if (!SDLUtils::Init("TheatrumMundi", w, h, "../resources/config/TheatrumMundi.resources.json")) {
		std::cerr << "Something went wrong while initializing SDLUtils"
			<< std::endl;
		return;
	}
	





	sdlutils().loadReasources("../resources/config/TheatrumMundiSounds.resources.json");

	

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return;

	}
	sdlutils().hideCursor();

	// Create the manager
	_log = new Log();
	_dataManager = new DataManager();
	dialogueReader = new ReadDialog(6);
	_mngr = new SceneManager();
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
	_mngr->init();
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

		audioManager().update();

		_mngr->update();

		checkCollisions();
		render();

		_mngr->refresh();
		if (_reset) reset();
		Uint32 frameTime = sdlutils().currRealTime() - startTime;
		
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);

		//cheats
#ifdef _DEBUG
		//load room1
		if (ihdlr.isKeyDown(SDL_SCANCODE_1)) {
			std::cout << "Load Room1Scene"<<std::endl;
			_mngr->loadScene(SceneName::ROOM_1);
			InputHandler::Instance()->clearState();
		}
		if (ihdlr.isKeyDown(SDL_SCANCODE_2)) {
			std::cout << "Load Room2Scene" << std::endl;
			_mngr->loadScene(SceneName::ROOM_2);
			InputHandler::Instance()->clearState();
		}
		if (ihdlr.isKeyDown(SDL_SCANCODE_3)) {
			std::cout << "Load Room3Scene" << std::endl;
			_mngr->loadScene(SceneName::ROOM_3);
			InputHandler::Instance()->clearState();
		}
		if (ihdlr.isKeyDown(SDL_SCANCODE_S)) {
			_mngr->ResolveActScene();
		}
#endif // _DEBUG

	}
}

void Game::loadResouces(int part)
{
	//loadResouces
#ifndef _LOADALLRESOURCES

	switch (part)
	{
	case INITIAL_MENU:
		if (loadedResources != part) {
			sdlutils().ClearMaps();
			sdlutils().loadReasources("../resources/config/TheatrumMundiInitialMenu.resources.json");
			loadedResources = part;
		}
		break;
	case TUTORIAL_SCENE:
		if (loadedResources != part) {
			sdlutils().ClearMaps();
			sdlutils().loadReasources("../resources/config/TheatrumMundiTutorial.resources.json");

			loadedResources = part;
		}
		break;
	case SceneName::MIDDLE_ROOM:
		if (loadedResources != part) {
			int aux = Game::Instance()->getDataManager()->GetActualScene();
			switch (aux)
			{
			case SceneCount::MIDDLEROOM1:
				if (loadedResources != part) {
					sdlutils().ClearMaps();

					sdlutils().loadReasources("../resources/config/TheatrumMundiRoom1.resources.json");
					loadedResources = 4;
				}
				break;
			case SceneCount::MIDDLEROOM2:
				if (loadedResources != part) {
					sdlutils().ClearMaps();

					sdlutils().loadReasources("../resources/config/TheatrumMundiRoom2.resources.json");
					loadedResources = 5;
				}
				break;
			case SceneCount::MIDDLEROOM3:
				if (loadedResources != part) {
					sdlutils().ClearMaps();
					sdlutils().loadReasources("../resources/config/TheatrumMundiRoom3.resources.json");
					loadedResources = 6;
				}
				break;
			default:
				break;
			}
		}
		break;

	case SceneName::CREDITS:
		if (loadedResources != part) {
			sdlutils().ClearMaps();

			sdlutils().loadReasources("../resources/config/TheatrumMundiCredits.resources.json");
			loadedResources = part;
		}
		break;
	
	default:
		if (loadedResources != part) {
			sdlutils().ClearMaps();
			sdlutils().loadReasources("../resources/config/TheatrumMundiRoom3.resources.json");
			loadedResources = 6;
		}


#ifdef DEBUG
		std::cout << "NOT LOAD MORE RESOURCES" << std::endl;
#endif // DEBUG
		
		break;
	}
#endif // !_LOADALLRESOURCES

}

void Game::reset()
{
	_reset = false;
	//Reset instances
	Game::Instance()->getLog()->ResetLog();
	Game::Instance()->getDataManager()->ResetDataManager();
	Game::Instance()->getSceneManager()->ResetSceneManager();

	Game::Instance()->getReadDialogue()->ResetReader();

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
ReadDialog* Game::getReadDialogue()
{
	return dialogueReader;
}
void Game::setReset()
{
	_reset = true;
}
Log* Game::getLog()
{
	return _log;
}

void Game::checkCollisions() {
	
}
