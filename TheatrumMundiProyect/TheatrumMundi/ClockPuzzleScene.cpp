#include "ClockPuzzleScene.h"

#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"


ClockPuzzleScene::ClockPuzzleScene() : ScenePuzzleTemplate()
{
}

ClockPuzzleScene::~ClockPuzzleScene()
{
}

void ClockPuzzleScene::init()
{


	if (!isStarted) {

		//create the clock
		auto _clockShape = entityManager->addEntity();
		auto _clockShapeTransform = entityManager->addComponent<Transform>(_clockShape, Vector2D(600, 300), Vector2D(0, 0), 200, 200, 0);
		entityManager->addComponent<Image>(_clockShape, &sdlutils().images().at("clockShape"));

		entityManager->addComponent<RectArea2D>(_clockShape);

		ClickComponent* clockClick = entityManager->addComponent<ClickComponent>(_clockShape);
		clockClick->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });

		TriggerComponent* clockTrigger = entityManager->addComponent<TriggerComponent>(_clockShape);
		clockTrigger->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		clockTrigger->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });


		//create the clock hands : minute
		auto _clockMin = entityManager->addEntity();
		auto _clockMinTransform = entityManager->addComponent<Transform>(_clockMin, Vector2D(100, 150), Vector2D(0, 0), 50, 170, 0);
		entityManager->addComponent<Image>(_clockMin, &sdlutils().images().at("clockMinArrow"));

		entityManager->addComponent<RectArea2D>(_clockMin);

		ClickComponent* clockMinClick = entityManager->addComponent<ClickComponent>(_clockMin);
		clockMinClick->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });

		TriggerComponent* clockMinTrigger = entityManager->addComponent<TriggerComponent>(_clockMin);
		clockMinTrigger->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		clockMinTrigger->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });

		DragComponent* drgMin = entityManager->addComponent<DragComponent>(_clockMin);
		drgMin->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });


		//create the clock hands : hour
		auto _clockHor = entityManager->addEntity();
		auto _clockHorTransform = entityManager->addComponent<Transform>(_clockHor, Vector2D(160, 170), Vector2D(0, 0), 50, 130, 0);
		entityManager->addComponent<Image>(_clockHor, &sdlutils().images().at("clockMinArrow"));

		entityManager->addComponent<RectArea2D>(_clockHor);

		ClickComponent* clockHorClick = entityManager->addComponent<ClickComponent>(_clockHor);
		clockHorClick->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });

		TriggerComponent* clockHorTrigger = entityManager->addComponent<TriggerComponent>(_clockHor);
		clockHorTrigger->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		clockHorTrigger->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });
		

		DragComponent* drgHor = entityManager->addComponent<DragComponent>(_clockHor);
		drgHor->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });

	}

}

void ClockPuzzleScene::refresh()
{
}

void ClockPuzzleScene::unload()
{
}

