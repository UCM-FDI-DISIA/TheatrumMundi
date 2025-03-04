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
			startDialogue(SalaIntermediaEvento1);
		};
	roomEvent[PipePuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(PipePuzzleSnc,this);
		};
	roomEvent[PipePuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(0);
		};
	roomEvent[BooksPuzzleScn] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(BooksPuzzleScn, this);
		};
	roomEvent[BooksPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(1);
		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(ClockPuzzleSnc, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(2);
		};
	roomEvent[TeaCupPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(TeaCupPuzzleSnc, this);
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

		auto LivingBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(0 - sdlutils().width(), 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingBackground));

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
