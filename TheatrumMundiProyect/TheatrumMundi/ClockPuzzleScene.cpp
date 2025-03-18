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
#include "../src/Components/LogComponent.h"

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
	SetInventory(sr->GetInventory());

	if (!isStarted) {
		isStarted = true;

		//Register scene in dialogue manager
		Game::Instance()->getDialogueManager()->setScene(this);

		
		//All Screen: Object to detect click on screen. Used to read displayed dialogue.
		auto _screenDetect = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->addComponent<Image>(_screenDetect, &sdlutils().images().at("fondoPruebaLog"), 100); //background log
		entityManager->addComponent<RectArea2D>(_screenDetect, areaLayerManager);
		entityManager->addComponent<TriggerComponent>(_screenDetect);
		entityManager->setActive(_screenDetect, false);

		

		//Create dialogue text entity. Object that renders dialogue Text on Screen
		auto _textbackground = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_textbackground, Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0);
		entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
		entityManager->addComponent<RectArea2D>(_textbackground, areaLayerManager);

		entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
			{
				if (!logActive) {
					//read dialogue only if it has to
					if (Game::Instance()->getDialogueManager()->getDisplayOnProcess())
					{
						Game::Instance()->getDialogueManager()->ReadDialogue(Puzzle3);
					}
					else
					{
						_textbackground->getMngr()->setActive(_textbackground, false);
					}
				}
			});
		entityManager->addComponent<TriggerComponent>(_textbackground);
		entityManager->setActive(_textbackground, false);

		//CharacterImage
		auto characterimg = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(characterimg, Vector2D(0, 200), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
		auto imCh = entityManager->addComponent<Image>(characterimg, &sdlutils().images().at("Dialog"));

		Game::Instance()->getDialogueManager()->setCharacterImg(imCh);
		entityManager->setActive(characterimg, false);

		auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		entityManager->setActive(_textTest, false);


		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);
		
		startDialogue(Puzzle3);

		room = sr;
		AudioManager& a = AudioManager::Instance();
		Sound clockMinSound = sdlutils().soundEffects().at("aguja_minutero");
		Sound clockHorSound = sdlutils().soundEffects().at("aguja_horario");
		a.setVolume(clockMinSound,0.2);
		a.setVolume(clockHorSound, 0.2);

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
		clockCheckClick->connect(ClickComponent::JUST_CLICKED, [_buttonCheckTransform, sr, this]()
			{
				if (Check()) {

					sr->GetInventory()->addItem(new Hint("AAA", "Me lo puedo beber??", &sdlutils().images().at("AAA")));
					sr->GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(sr->GetEntityManager(), "AAA", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
					sr->GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);

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
#ifdef DEBUG
				std::cout << "WAAAAAAAAAA\n";
#endif // DEBUG

				_clockHorTransform->setRot(90);
				_actualHour = 90;
				_clockMinTransform->setRot(0);
				_actualMinute = 0;
			});


		//BackButton
		auto _backButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(_backButton);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, []()
			{
				Game::Instance()->getSceneManager()->popScene();
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
	if (_actualHour == 180 && _actualMinute == 180 && !solved)
	{
		solved = true;
		return true;
	}
	else 
	return false;
}

void ClockPuzzleScene::Win()
{
	room->resolvedPuzzle(7);
}

