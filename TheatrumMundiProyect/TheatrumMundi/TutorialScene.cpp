#include "TutorialScene.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "LogComponent.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"
#include "ClickableSpriteComponent.h"
#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"



#include "../src/components/WriteTextComponent.h"
#include "DialogueManager.h"


#include "GameSave.h"

TutorialScene::TutorialScene() : SceneRoomTemplate()
{
	dialogueManager = new DialogueManager(-1);
	roomEvent.resize(event_size);
	roomEvent[Dialog0] = [this]
		{
			startDialogue("Tutorial0");

		};
	roomEvent[Dialog1] = [this]
		{
			startDialogue("Tutorial1");

		};
	roomEvent[Dialog2] = [this]
		{
			startDialogue("Tutorial2");

		};
	roomEvent[Dialog3] = [this]
		{
			startDialogue("Tutorial3");

		};
	roomEvent[Dialog4] = [this]
		{
			startDialogue("Tutorial4");

		};
	roomEvent[Dialog5] = [this]
		{
			startDialogue("Tutorial5");

		};
	roomEvent[Dialog6] = [this]
		{
			startDialogue("Tutorial6");

		};
	roomEvent[Dialog7] = [this]
		{
			startDialogue("Tutorial7");

		};
	roomEvent[Dialog8] = [this]
		{
			startDialogue("Tutorial8");

		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		};
	roomEvent[TeleScene] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(TELE_PUZZLE, this);
		};
	roomEvent[Spoon] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("TeaCupSpoon", "Es una cuchara, que no lo ves o que", &sdlutils().images().at("TeaCupSpoon")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue("SalaIntermedia1");
		};
	roomEvent[MobileDialogue] = [this] {
		// WIP
		_eventToRead = Movil;
		startDialogue("Movil");
		};

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::init()
{

	if (!isStarted) {
		isStarted = true;
		finishallpuzzles = false;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		Sound puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		a.setVolume(puzzleButtonSound, 0.3);
		//Audio music
		Sound room1music = sdlutils().musics().at("room1music");
		a.setLooping(room1music, true);
		a.playSound(room1music);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);


		//Background and Scene scroll
		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	
		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);


		auto startRoom = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		
		auto startRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(startRoom));


		auto secondRoom = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(secondRoom));

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, startRoomScroll, ChangeRoomScroll]() {
			if (!startRoomScroll->isScrolling()) {
				//AudioManager::Instance().playSound(doorSound);
				startRoomScroll->Scroll(ScrollComponent::RIGHT);
			}
			});

		//test 
		auto botonTest = entityManager->addEntity();
		entityManager->addComponent<Transform>(botonTest, /*Vector2D(1348 / 2.8, 748 / 2)*/Vector2D(356 - 1349 - 6, 127), Vector2D(0, 0), 1470 / 4, 270 / 4, 0);
		entityManager->addComponent<Image>(botonTest, &sdlutils().images().at("TutorialButton"));

		entityManager->addComponent<RectArea2D>(botonTest);

		ClickComponent* clkTut = entityManager->addComponent<ClickComponent>(botonTest);
		clkTut->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			GameSave save;
			save.setTutoCompleted(true);
			save.Write("savegame.dat");
			});
		
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(botonTest));


		//Television
		auto television = entityFactory->CreateInteractableEntity(entityManager, "teleSinAntena", EntityFactory::RECTAREA, Vector2D(170 - 1349, 400), Vector2D(0, 0), 750 / 3, 760 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(television));
		entityManager->getComponent<ClickComponent>(television)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[TeleScene]();
			});




		//quit button
/*		auto botonBack = entityManager->addEntity();
		entityManager->addComponent<Transform>(botonBack, Vector2D(1349 - 110, 20), Vector2D(0, 0), 270 / 4, 270 / 4, 0);
		entityManager->addComponent<Image>(botonBack, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(botonBack);

		ClickComponent* clkBack = entityManager->addComponent<ClickComponent>(botonBack);
		clkBack->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			});
*/
		//setInteractable(botonBack);

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, _eventToRead);

		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);
		Game::Instance()->getLog()->addDialogueLineLog(" ", "3711");
		//Game::Instance()->getLog()->SetLogActive(false);
		entityManager->setActive(Game::Instance()->getLog(), true);

		//roomEvent[Dialog0];
		startDialogue("Tutorial0");

	}
	SDL_Delay(1000);

}

void TutorialScene::resolvedPuzzle(int i)
{
	/*
	if (i < 4) {
		int auxevent = event_size;
		if (i == 0)  auxevent = PipePuzzleRsv;
		else if (i == 1)  auxevent = BooksPuzzleRsv;
		else if (i == 2)  auxevent = ClockPuzzleRsv;
		else if (i == 3)  auxevent = TeaCupPuzzleRsv;
		roomEvent[auxevent]();
		puzzlesol[i] = true;
		//std::cout << "Puzle resuelto" << i << std::endl;
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		//if (aux) entityManager->setActive(body, true);
	}
	else {
#ifdef _DEBUG
		std::cout << i << " invalid index" << std::endl;
#endif
	}
	*/
}

void TutorialScene::refresh()
{
}

void TutorialScene::unload()
{
	entityManager->~EntityManager();
}


void TutorialScene::setInteractable(Entity* e) {
	
	e = entityManager->addEntity();
	entityManager->addComponent<Transform>(e, Vector2D(1349 - 110, 20), Vector2D(0, 0), 270 / 4, 270 / 4, 0);
	entityManager->addComponent<Image>(e, &sdlutils().images().at("B1"));

	entityManager->addComponent<RectArea2D>(e);
	
	
	ClickComponent* clkBack = entityManager->addComponent<ClickComponent>(e);
	clkBack->connect(ClickComponent::JUST_CLICKED, [this]() {

		//AudioManager::Instance().playSound(buttonSound);
		Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
		});
}

void TutorialScene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	std::cout << "acaboDialog";
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	//Check the act room to load a specific room
	setInteractable(botonBack);
	
	
}
