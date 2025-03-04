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

#include "AudioManager.h"

#include "SceneRoomTemplate.h"

ClockPuzzleScene::ClockPuzzleScene() : ScenePuzzleTemplate()
{
	_actualHour = 90;
	_actualMinute = 0;
}

ClockPuzzleScene::~ClockPuzzleScene()
{
}

void ClockPuzzleScene::init(SceneRoomTemplate* sr)
{


	if (!isStarted) {

		AudioManager& a = AudioManager::Instance();
		Sound clockMinSound = sdlutils().soundEffects().at("aguja_minutero");
		Sound clockHorSound = sdlutils().soundEffects().at("aguja_horario");
		a.setVolume(clockMinSound,0.2);
		a.setVolume(clockHorSound, 0.2);

		room = sr;
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
		auto _clockHorTransform = entityManager->addComponent<Transform>(_clockHor, Vector2D(695, 360), Vector2D(0, 0), 20, 60, 90);
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
		clockMinClick->connect(ClickComponent::JUST_CLICKED, [_clockMinTransform, clockMinSound, this]()
			{
#ifdef DEBUG
				std::cout << "CLICKED MINUTERO\n";
#endif // DEBUG

				AudioManager::Instance().playSound(clockMinSound);
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
		clockHorClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform, clockHorSound, this]()
			{
#ifdef DEBUG
				std::cout << "CLICKED HORARIO\n";
#endif // DEBUG

				AudioManager::Instance().playSound(clockHorSound);
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
				if (Check()) {
#ifdef DEBUG
					std::cout << "wii";
#endif // DEBUG
					Win();
					
				}
			});


		//create the buttons: reset button
		auto _buttonResetPuzzle = entityManager->addEntity();
		auto _buttonRessetPuzzleTransform = 
		entityManager->addComponent<Transform>(_buttonResetPuzzle, Vector2D(1200, 150), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonResetPuzzle, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonResetPuzzle);


		ClickComponent* clockResetClick = entityManager->addComponent<ClickComponent>(_buttonResetPuzzle);
		clockResetClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform,_clockMinTransform, this]()
			{
				std::cout << "WAAAAAAAAAA\n";

				_clockHorTransform->setRot(90);
				_actualHour = 90;
				_clockMinTransform->setRot(0);
				_actualMinute = 0;
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

void ClockPuzzleScene::Win()
{
	room->resolvedPuzzle(7);
}

