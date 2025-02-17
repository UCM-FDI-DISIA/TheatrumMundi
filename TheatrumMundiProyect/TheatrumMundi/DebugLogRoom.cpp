#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "Log.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate()
{
	_sceneLog = new Log();
}

DebugLogRoom::~DebugLogRoom()
{
}

void DebugLogRoom::init()
{
	if (!isStarted) {
		auto _fighter = entityManager->addEntity();
		auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));

		//add debug lines to log
		_sceneLog->addDialogueLineLog("autorX", "hola que tal");
		_sceneLog->addDialogueLineLog("autorY", "segunda linea");
		_sceneLog->addDialogueLineLog("/", "/");

		_sceneLog->showLog();
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
	delete _sceneLog;
}
