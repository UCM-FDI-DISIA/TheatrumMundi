#include "Room1.h"
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

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"


Room1Scene::Room1Scene(): SceneRoomTemplate()
{
	roomEvent.resize(event_size);
	roomEvent[InitialDialogue] = [this] 
	{
		startDialogue(SalaIntermediaEvento1);
	};
	roomEvent[CorpseDialogue] = [this]
		{
			std::cout << "semurio";
			startDialogue(SalaIntermediaEvento1);
		};
	roomEvent[PipePuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(PIPE_PUZZLE,this);
		};
	roomEvent[PipePuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(0);
		};
	roomEvent[BooksPuzzleScn] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(BOOKS_PUZZLE, this);
		};
	roomEvent[BooksPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(1);
		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(2);
		};
	roomEvent[TeaCupPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(TEA_CUP_PUZZLE, this);
		};
	roomEvent[TeaCupPuzzleRsv] = [this] {
		// InventoryLogic
		entityManager->removeComponent<ClickComponent>(puzzleptr[4]);
		};
	roomEvent[Spoon] = [this] {
		// InventoryLogic
		};
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue(SalaIntermediaEvento1);
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		};
	
}

Room1Scene::~Room1Scene()
{
}

void Room1Scene::init()
{
	
	if (!isStarted) {

		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width())/12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - sdlutils().width(), 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));

		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		auto StudyBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyBackground));

		//StudyRoom (Right)
		auto LivingBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(- sdlutils().width()-6, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingBackground));

		auto Corspe = entityFactory->CreateInteractableEntity(entityManager, "Corspe",EntityFactory::RECTAREA, Vector2D(1000, 422), Vector2D(0, 0), 268, 326, 0, areaLayerManager,EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Corspe));
		entityManager->getComponent<ClickComponent>(Corspe)->connect(ClickComponent::JUST_CLICKED, [this]() {roomEvent[CorpseDialogue]();});

		auto Timetable = entityFactory->CreateInteractableEntity(entityManager, "Timetable", EntityFactory::RECTAREA, Vector2D(1173, 267), Vector2D(0, 0), 138, 182, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Timetable));
		
		auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "calendearZoom", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::ZOOMOBJ);
		
		entityManager->setActive(_calendearZoom, false);
		auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "exit", entityFactory->RECTAREA, Vector2D(10, 10), Vector2D(0, 0), 50, 50, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		ClickComponent* click2 = entityManager->addComponent<ClickComponent>(_quitButton);
		//Exit Calendear Zoom
		click2->connect(ClickComponent::JUST_CLICKED, [this, _calendearZoom, Timetable, _quitButton]()
			{
				_calendearZoom->getMngr()->setActive(_calendearZoom, false);
				Timetable->getMngr()->setActive(Timetable, true);
				_quitButton->getMngr()->setActive(_quitButton, false);
			});
		entityManager->setActive(_quitButton, false);
		entityManager->getComponent<ClickComponent>(Timetable)->connect(ClickComponent::JUST_CLICKED, [this, Timetable,_calendearZoom,_quitButton]()
			{
				//this->startDialogue(Calendario);
				Timetable->getMngr()->setActive(Timetable, false);
				entityManager->setActive(_calendearZoom,true);
				entityManager->setActive(_quitButton, true);
			});

		auto Clock = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(828, 95), Vector2D(0, 0), 142, 553, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Clock));
		entityManager->getComponent<ClickComponent>(Clock)->connect(ClickComponent::JUST_CLICKED, [this]() {roomEvent[ClockPuzzleSnc]();});

		auto Shelf = entityFactory->CreateInteractableEntity(entityManager, "Shelf", EntityFactory::RECTAREA, Vector2D(214, 96), Vector2D(0, 0), 191, 548, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Shelf));
		entityManager->getComponent<ClickComponent>(Shelf)->connect(ClickComponent::JUST_CLICKED, [this]() {roomEvent[BooksPuzzleScn]();});
		//LivingRoom (Left)

		auto Tubes = entityFactory->CreateInteractableEntity(entityManager, "Tubes", EntityFactory::RECTAREA, Vector2D(356-sdlutils().width() - 6, 127), Vector2D(0, 0), 616, 336, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Tubes));
		entityManager->getComponent<ClickComponent>(Tubes)->connect(ClickComponent::JUST_CLICKED, [this]() {roomEvent[PipePuzzleSnc]();});

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1Button,StudyBackgroundScroll]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				StudyBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
			});

		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		ChangeRoom2Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom2Button, StudyBackgroundScroll]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				StudyBackgroundScroll->Scroll(ScrollComponent::LEFT);
			}
			});
		
		
	


		//Create log
		auto _log = entityManager->addEntity();
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"), 200); //background log

		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent

		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog =
			entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("BASE"), colorText, logComp->getLogList()); //write text component

		_log->getMngr()->setActive(_log, false); //hide log at the beggining

		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logComp);


		roomEvent[LOGENABLE] = [this] {
			//activate log
			/*_log->getMngr()->setActive(_log, true);
			logActive = true;

			//activate close log button
			_closeLogButton->getMngr()->setActive(_closeLogButton, true);

			//disable open log button
			_openLogButton->getMngr()->setActive(_openLogButton, false);
			*/
			};
		roomEvent[LOGDESABLE] = [this,_log] {
			//disable log
			_log->getMngr()->setActive(_log, false);
			logActive = false;

			//activate open log button
			//_openLogButton->getMngr()->setActive(_openLogButton, true);

			//disable close log button
			//_closeLogButton->getMngr()->setActive(_closeLogButton, false);
			};
	}
	//SDL_Delay(1000);
	//_loadimg->getMngr()->setActive(_loadimg,false);
	
}

void Room1Scene::resolvedPuzzle(int i)
{
	if (i == ClockPuzzleRsv || i == PipePuzzleRsv || i == BooksPuzzleRsv || i == TeaCupPuzzleRsv) {
		roomEvent[i]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		if (aux) entityManager->setActive(body, true);
	}
else {
#ifdef _DEBUG
	std::cout << i << " invalid index" << std::endl;
#endif
}
}

void Room1Scene::refresh()
{
}

void Room1Scene::unload()
{
	entityManager->~EntityManager();
}
