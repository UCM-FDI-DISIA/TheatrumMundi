#include "CalendearMobileTestScene.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"

CalendearMobileTestScene::CalendearMobileTestScene()
{
}

CalendearMobileTestScene::~CalendearMobileTestScene()
{
}

void CalendearMobileTestScene::init()
{

	auto _calendearButton = entityManager->addEntity();
	entityManager->addComponent<Transform>(_calendearButton, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);

	entityManager->addComponent<RectArea2D>(_calendearButton);
	entityManager->addComponent<Image>(_calendearButton,&sdlutils().images().at("calendear"));
	ClickComponent* click = entityManager->addComponent<ClickComponent>(_calendearButton);
	click->connect(ClickComponent::JUST_CLICKED, [this, _calendearButton]()
		{
			auto _calendearZoom = entityManager->addEntity();
			entityManager->addComponent<Image>(_calendearZoom, &sdlutils().images().at("calendearZoom"));
			Game::Instance()->getDialogueManager()->ReadDialogue(Calendario);
		});
}

void CalendearMobileTestScene::refresh()
{
}

void CalendearMobileTestScene::unload()
{
}
