#include "SceneRoomTemplate.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include <iostream>

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
	auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(0,0), Vector2D(0, 0), 500, 500, 0);
	entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));

	entityManager->addComponent<RectArea2D>(_fighter);

	ClickComponent* clk = entityManager->addComponent<ClickComponent>(_fighter);
	clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });
}

void SceneRoomTemplate::unload()
{
	
}



