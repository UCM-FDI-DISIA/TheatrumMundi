#include "Room1.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/Components/ScrollComponent.h"

Room1::Room1(): SceneRoomTemplate()
{
}

Room1::~Room1()
{
}

void Room1::init()
{
	if (!isStarted) {
		auto _fighter = entityManager->addEntity();
		auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_fighter);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_fighter);
		clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });

		TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(_fighter);
		trg->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		trg->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });

		DragComponent* drg = entityManager->addComponent<DragComponent>(_fighter);
		drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });

		//ScrolledComponent

		//BOTON
		auto _button = entityManager->addEntity();
		auto _buttonTransform = entityManager->addComponent<Transform>(_button, Vector2D(10, 10), Vector2D(1, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_button, &sdlutils().images().at("prueba"));

		//ScrollComponent
		ScrollComponent* _buttonScroll = entityManager->addComponent<ScrollComponent>(_button);

		ClickComponent* clkb = entityManager->addComponent<ClickComponent>(_button);
		clkb->connect(ClickComponent::JUST_CLICKED, []() { });
	}
}

void Room1::refresh()
{
}

void Room1::unload()
{
}
