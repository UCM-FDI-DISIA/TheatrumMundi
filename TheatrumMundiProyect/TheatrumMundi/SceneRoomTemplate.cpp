#include "SceneRoomTemplate.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"


SceneRoomTemplate::SceneRoomTemplate(): SceneTemplate()
{
}

SceneRoomTemplate::~SceneRoomTemplate()
{
	unload();
}

void SceneRoomTemplate::init()
{
	auto _fighter = entityManager->addEntity();
	auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(10,10), Vector2D(0, 0), 500, 500, 0);
	entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));
}

void SceneRoomTemplate::unload()
{
	
}



