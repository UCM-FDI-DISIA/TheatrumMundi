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
	_actualHour = 0;
	_actualMinute = 0;
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

		//create the clock hands : minute
		auto _clockMin = entityManager->addEntity();
		auto _clockMinTransform = entityManager->addComponent<Transform>(_clockMin, Vector2D(680, 360), Vector2D(0, 0), 20, 70, 0);
		entityManager->addComponent<Image>(_clockMin, &sdlutils().images().at("clockMinArrow"));

		entityManager->addComponent<RectArea2D>(_clockMin);


		//create the clock hands : hour
		auto _clockHor = entityManager->addEntity();
		auto _clockHorTransform = entityManager->addComponent<Transform>(_clockHor, Vector2D(695, 360), Vector2D(0, 0), 20, 60, 0);
		//auto _clockHorTransform = entityManager->addComponent<Transform>(_clockHor, Vector2D(200, 350), Vector2D(0, 0), 20, 60, 0);
		entityManager->addComponent<Image>(_clockHor, &sdlutils().images().at("clockHorArrow"));

		entityManager->addComponent<RectArea2D>(_clockHor);


		//create the buttons: min
		auto _buttonMin = entityManager->addEntity();
		auto _buttonMinTransform = entityManager->addComponent<Transform>(_buttonMin, Vector2D(200, 600), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonMin, &sdlutils().images().at("clockMinButton"));

		entityManager->addComponent<RectArea2D>(_buttonMin);

		_actualMinute;

		ClickComponent* clockMinClick = entityManager->addComponent<ClickComponent>(_buttonMin);
		clockMinClick->connect(ClickComponent::JUST_CLICKED, [_clockMinTransform, this]()
			{
				std::cout << "CLICKED\n";
				_clockMinTransform->setRot(_clockMinTransform->getRot() + 15);
				_actualMinute += 15;
				if (_actualMinute == 360) _actualMinute = 0;

			});
			

		//create the buttons: hor
		auto _buttonHor = entityManager->addEntity();
		auto _buttonHorTransform = entityManager->addComponent<Transform>(_buttonHor, Vector2D(200, 550), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonHor, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonHor);


		ClickComponent* clockHorClick = entityManager->addComponent<ClickComponent>(_buttonHor);
		clockHorClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform, this]()
			{
				std::cout << "CLICKED\n";
				_clockHorTransform->setRot(_clockHorTransform->getRot() + 30);
				_actualHour += 30;
				if (_actualHour == 360) _actualHour = 0;
			});


		//create the buttons: check
		auto _buttonCheck = entityManager->addEntity();
		auto _buttonCheckTransform = entityManager->addComponent<Transform>(_buttonCheck, Vector2D(100, 550), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonCheck, &sdlutils().images().at("clockCheckButton"));

		entityManager->addComponent<RectArea2D>(_buttonCheck);


		ClickComponent* clockCheckClick = entityManager->addComponent<ClickComponent>(_buttonCheck);
		clockCheckClick->connect(ClickComponent::JUST_CLICKED, [_buttonCheckTransform, this]()
			{
				std::cout << "CLICKED\n";
				if (Check()) std::cout << "wii";
			});

	}

}

void ClockPuzzleScene::refresh()
{
}

void ClockPuzzleScene::unload()
{
}

bool ClockPuzzleScene::Check()
{
	if (_actualHour == 180 && _actualMinute == 180) return true;
	else 
	return false;
}

