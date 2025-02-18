// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include <Windows.h>


Game* Game::_instance = nullptr;
Game::Game() {

}
Game* Game::Instance()
{
		if (_instance == nullptr)
			_instance = new Game();
		return _instance;
}
Game::~Game() {
	delete _mngr;

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

void Game::init() {

	// initialize the SDL singleton
	//if (!SDLUtils::Init("TheatrumMundi",GetSystemMetrics(SM_CXSCREEN) ,GetSystemMetrics(SM_CYSCREEN), "../resources/config/TheatrumMundi.resources.json")) {
	if (!SDLUtils::Init("TheatrumMundi", 1920, 1080, "../resources/config/TheatrumMundi.resources.json")) {
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
	_Dmngr = new DialogueManager();
	_mngr = new SceneManager();
	
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	// reset the time before starting - so we calculate correct
	// delta-time in the first iteration
	//
	sdlutils().resetTime();

	while (!exit) {
		// store the current time -- all game objects should use this time when
		// then need to the current time. They also have accessed to the time elapsed
		// between the last two calls to regCurrTime().
		Uint32 startTime = sdlutils().regCurrTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		_mngr->update();

		checkCollisions();

		sdlutils().clearRenderer();
		_mngr->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

SceneManager* Game::getSceneManager()
{
	assert(_mngr != nullptr);
	return _mngr;
}

DialogueManager* Game::getDialogueManager()
{
	assert(_Dmngr != nullptr);
	return _Dmngr;
}

void Game::checkCollisions() {
	
}
