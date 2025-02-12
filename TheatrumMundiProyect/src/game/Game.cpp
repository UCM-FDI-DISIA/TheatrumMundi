// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/Deceleration.h"
#include "../components/GameInfoMsgs.h"
#include "../components/GameState.h"
#include "../components/Image.h"
#include "../components/FighterCtrl.h"
#include "../components/RectangleViewer.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

using ecs::Manager;

Game::Game() : _mngr(nullptr), _gameState(nullptr) {}
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
	if (!SDLUtils::Init("Asteroids", 800, 600,"resources/config/pingpong.resources.json")) {

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
	_mngr = new Manager();

	// create SpaceShip entity
	auto _fighter = _mngr->addEntity();
	auto _fighterTransform = _mngr->addComponent<Transform>(_fighter, Vector2D(sdlutils().width() / 2, sdlutils().width() / 2), Vector2D(0, 0), 50, 50, 0);
	_mngr->addComponent<Image>(_fighter, &sdlutils().images().at("fighter"));
	_mngr->addComponent<Deceleration>(_fighter,_fighterTransform);
	_mngr->addComponent<FighterCtrl>(_fighter,5.0,_fighterTransform);
	_mngr->addComponent <ShowAtOppositeSide> (_fighter, _fighterTransform);
	// create the Asteroid 
	
	// create game control entity
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
		_mngr->refresh();

		checkCollisions();

		sdlutils().clearRenderer();
		_mngr->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

void Game::checkCollisions() {
	/*if (_gameState->getState() != GameState::RUNNING)
		return;

	bool ballCollidesWithPaddle = false;

	for (auto e : _mngr->getEntities(ecs::grp::PADDLES)) {
		auto paddleTr_ = _mngr->getComponent<Transform>(e);
		ballCollidesWithPaddle = Collisions::collides(paddleTr_->getPos(),
				paddleTr_->getWidth(), paddleTr_->getHeight(), ballPos,
				ballWidth, ballHeight);

		if (ballCollidesWithPaddle)
			break;
	}

	if (ballCollidesWithPaddle) {

		// change the direction of the ball, and increment the speed
		auto &vel = _ballTr->getVel(); // the use of & is important, so the changes goes directly to the ball
		vel.setX(-vel.getX());
		vel = vel * 1.2f;

		// play some sound
		sdlutils().soundEffects().at("paddle_hit").play();
	} else if (_ballTr->getPos().getX() < 0)
		_gameState->onBallExit(GameState::LEFT);
	else if (_ballTr->getPos().getX() + _ballTr->getWidth()
			> sdlutils().width())
		_gameState->onBallExit(GameState::RIGHT);*/

}
