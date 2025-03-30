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
#include "ClickableSpriteComponent.h"
#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"



#include "../src/components/WriteTextComponent.h"
#include "DialogueManager.h"
Room1Scene::Room1Scene() : SceneRoomTemplate(), _eventToRead("SalaIntermedia1")
{
	dialogueManager = new DialogueManager(1);
	roomEvent.resize(event_size);
	roomEvent[InitialDialogue] = [this]
		{
			startDialogue("SalaIntermedia1");

		};
	roomEvent[CorpseDialogue] = [this]
		{
			roomEvent[ResolveCase]();



#ifdef DEBUG
			std::cout << "semurio";
#endif // DEBUG

			_eventToRead = Cadaver;
			startDialogue("Cadaver");
		};
	roomEvent[PipePuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(PIPE_PUZZLE,this);
		};
	roomEvent[PipePuzzleRsv] = [this] {
		// InventoryLogic
		
		};
	roomEvent[BooksPuzzleScn] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(BOOKS_PUZZLE, this);
		};
	roomEvent[BooksPuzzleRsv] = [this] {
		// InventoryLogic
		
		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
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
		GetInventory()->addItem(new Hint("TeaCupSpoon", "Es una cuchara, que no lo ves o que", &sdlutils().images().at("TeaCupSpoon")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA,GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};
	/*roomEvent[Boa1] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("boa1", "aaaa", &sdlutils().images().at("boa1")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};*/
	/*roomEvent[Boa2] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("boa2", "shhhh", &sdlutils().images().at("boa2")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};*/
	/*roomEvent[Hanni] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("hanni", "D:", &sdlutils().images().at("Hanni")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Hanni", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};*/
	/*roomEvent[Doku] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("AAA", "veneno aaaa", &sdlutils().images().at("AAA")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "AAA", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};*/
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue("SalaIntermedia1");
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		Game::Instance()->getSceneManager()->popScene();
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		Game::Instance()->getSceneManager()->popScene();
		};
	roomEvent[MobileDialogue] = [this] {
		// WIP
		_eventToRead = Movil;
		startDialogue("Movil");
		};
	
}

Room1Scene::~Room1Scene()
{
}



void Room1Scene::init()
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
		Sound doorSound = sdlutils().soundEffects().at("puerta");
		//Audio music
		Sound room1music = sdlutils().musics().at("room1music");
		a.setLooping(room1music, true);
		a.playSound(room1music);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);

		
		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width())/12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - 1349, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));
		
		auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this, _quitButton,buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(_quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			});
		entityManager->setActive(_quitButton, false);
		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		auto StudyBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyBackground));

		//StudyRoom (Right)
		auto LivingBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(- 1349-6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingBackground));


		//set the scene the variant is 
		Game::Instance()->getDataManager()->SetSceneCount(ROOM1);
		//get actual variant
		int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(ROOM1);
		std::cout << "VARIANTE"<<variantAct << std::endl;
	
		auto possibleButton = entityFactory->CreateInteractableEntity(entityManager, "posible", EntityFactory::RECTAREA, Vector2D(500, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(possibleButton, false);
		entityManager->getComponent<ClickComponent>(possibleButton)->connect(ClickComponent::JUST_CLICKED, [this,variantAct]() {

			//if its the not correct variant one dies
			if (variantAct!=0)
			{
				Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
				std::cout << "NO CORRECTA DE POS" << std::endl;


			}
		
		
			//change to intermediate room

			});
		auto noPossibleButton = entityFactory->CreateInteractableEntity(entityManager, "noPosible", EntityFactory::RECTAREA, Vector2D(600, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(noPossibleButton, false);
		entityManager->getComponent<ClickComponent>(noPossibleButton)->connect(ClickComponent::JUST_CLICKED, [this,variantAct]() {

			//if its the not correct variant one dies
			if (variantAct != 1||2)
			{
				std::cout << "NO CORRECTA DE NO POS" << std::endl;
				Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
			}

			//change to intermediate room



			});

		auto resolveButton = entityFactory->CreateInteractableEntity(entityManager, "resolve", EntityFactory::RECTAREA, Vector2D(0, 500), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(resolveButton, false);
		


		auto noResolveButton = entityFactory->CreateInteractableEntity(entityManager, "noResolve", EntityFactory::RECTAREA, Vector2D(0, 600), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(noResolveButton, false);
	
		
		//appears when the 3 puzzles have been resolved
		auto readyToResolveButton = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA, Vector2D(400, 400), Vector2D(0, 0), 400, 400, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);

		entityManager->getComponent<ClickComponent>(readyToResolveButton)->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton,resolveButton, readyToResolveButton]() {

			entityManager->setActive(noResolveButton, true);
			entityManager->setActive(resolveButton, true);
			entityManager->setActive(readyToResolveButton, false);
			
		});
		entityManager->setActive(readyToResolveButton, false);
		entityManager->setActive(readyToResolveButton, false);
		entityManager->getComponent<ClickComponent>(resolveButton)->connect(ClickComponent::JUST_CLICKED, [this, possibleButton, noPossibleButton, readyToResolveButton,resolveButton,noResolveButton]() {

			entityManager->setActive(noPossibleButton, true);
			
			entityManager->setActive(readyToResolveButton, false);
			entityManager->setActive(possibleButton, true);

			entityManager->setActive(resolveButton, false);
			entityManager->setActive(noResolveButton, false);

			});

		entityManager->getComponent<ClickComponent>(noResolveButton)->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToResolveButton]() {


			entityManager->setActive(noResolveButton, false);
			entityManager->setActive(resolveButton, false);
			entityManager->setActive(readyToResolveButton, true);



			});


		//Mobile

		auto Mobile = entityFactory->CreateInteractableEntity(entityManager, "mobileSprite", EntityFactory::RECTAREA, Vector2D(1250, 500), Vector2D(0, 0), 245/3, 123/3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Mobile));

		auto _mobileZoom = entityFactory->CreateImageEntity(entityManager, "mobileBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		entityManager->setActive(_mobileZoom, false);
		
		entityManager->getComponent<ClickComponent>(Mobile)->connect(ClickComponent::JUST_CLICKED, [this, _mobileZoom, Mobile, _quitButton]()
			{
				entityManager->setActive(_mobileZoom, true);
				entityManager->setActive(_quitButton, true);
				roomEvent[MobileDialogue]();
			});


		auto Timetable = entityFactory->CreateInteractableEntity(entityManager, "Timetable", EntityFactory::RECTAREA, Vector2D(1173, 267), Vector2D(0, 0), 138, 182, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Timetable));
		
		auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "TimetableZoom", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		entityManager->setActive(_calendearZoom, false);
		
		entityManager->setActive(_quitButton, false);
		entityManager->getComponent<ClickComponent>(Timetable)->connect(ClickComponent::JUST_CLICKED, [this, Timetable,_calendearZoom,_quitButton]()
			{
				//this->startDialogue(Calendario);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
				Timetable->getMngr()->setActive(Timetable, false);
				entityManager->setActive(_calendearZoom,true);
				entityManager->setActive(_quitButton, true);
			});

		//TeaCup
		auto TeaCup = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSprite", EntityFactory::RECTAREA, Vector2D(700, 265), Vector2D(0, 0), 165/3, 176/3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(TeaCup));
		entityManager->getComponent<ClickComponent>(TeaCup)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[TeaCupPuzzleSnc](); 
			});


		auto Clock = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(828, 95), Vector2D(0, 0), 142, 553, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Clock));
		entityManager->getComponent<ClickComponent>(Clock)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[ClockPuzzleSnc]();
			});

		auto Shelf = entityFactory->CreateInteractableEntity(entityManager, "Shelf", EntityFactory::RECTAREA, Vector2D(214, 96), Vector2D(0, 0), 191, 548, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Shelf));
		entityManager->getComponent<ClickComponent>(Shelf)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[BooksPuzzleScn]();
			});
		

		//LivingRoom (Left)

		auto Tubes = entityFactory->CreateInteractableEntity(entityManager, "Tubes", EntityFactory::RECTAREA, Vector2D(356-1349 - 6, 127), Vector2D(0, 0), 616, 336, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Tubes));
		entityManager->getComponent<ClickComponent>(Tubes)->connect(ClickComponent::JUST_CLICKED, [this,puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[PipePuzzleSnc]();
			});

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1Button,StudyBackgroundScroll, doorSound, ChangeRoomScroll]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				StudyBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
			});

		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		ChangeRoom2Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom2Button, StudyBackgroundScroll, doorSound]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				StudyBackgroundScroll->Scroll(ScrollComponent::LEFT);
			}
			});


		auto _corpseZoom = entityFactory->CreateImageEntity(entityManager, "CorspeBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		entityManager->addComponent<RectArea2D>(_corpseZoom, areaLayerManager);
		entityManager->setActive(_corpseZoom, false);

		body = entityFactory->CreateInteractableEntity(entityManager, "Corspe", EntityFactory::RECTAREA, Vector2D(1000, 422), Vector2D(0, 0), 268, 326, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(body));
		entityManager->getComponent<ClickComponent>(body)->connect(ClickComponent::JUST_CLICKED, [this, _corpseZoom, _quitButton]() {
			_corpseZoom->getMngr()->setActive(_corpseZoom, true);
			entityManager->setActive(_quitButton, true);
			if (!finishallpuzzles)roomEvent[CorpseDialogue]();
			else roomEvent[ResolveBottons]();

			});

		// Quit corpse zoom sbutton
		areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(_quitButton)->getLayerPos());

		//UI
		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager,"B3", EntityFactory::RECTAREA ,Vector2D(20,20), Vector2D(0,0), 90,90,0,areaLayerManager,EntityFactory::NODRAG ,ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		//Inventory
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
		auto buttonInventory = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3 ,20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		ClickComponent* buttonInventoryClick = entityManager->getComponent<ClickComponent>(buttonInventory);
		buttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound,InventoryBackground, upButton, downButton, buttonInventory]() {
			AudioManager::Instance().playSound(buttonSound);
			GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

			// If the inventory is active, activate the items
			if (GetInventory()->getActive()) {
				entityManager->setActive(InventoryBackground, true);

				buttonInventory->getMngr()->getComponent<Transform>(buttonInventory)->getPos().setX(925);
				//change the position of the log button

				entityManager->setActive(downButton, true);
				entityManager->setActive(upButton, true);

				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) {
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
				}
			}
			else {
				entityManager->setActive(InventoryBackground, false);

				entityManager->setActive(downButton, false);
				entityManager->setActive(upButton, false);

				buttonInventory->getMngr()->getComponent<Transform>(buttonInventory)->getPos().setX(60 + 268 / 3);
				//change the position of the log button

				// its okay to use the first item as the first item to show??
				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) {
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Desactivate the hints
				}
			}

		});

		
		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton]() {

			AudioManager::Instance().playSound(buttonSound);
			scrollInventory(1);
		});

		//Test obj 1
		/*auto boa1 = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 620), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(boa1));
		entityManager->getComponent<ClickComponent>(boa1)->connect(ClickComponent::JUST_CLICKED, [this, boa1]() {
			boa1->getMngr()->setActive(boa1, false);
			roomEvent[Boa1]();
			});*/
		//Test obj 2
		/*auto boa2 = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 700), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(boa2));
		entityManager->getComponent<ClickComponent>(boa2)->connect(ClickComponent::JUST_CLICKED, [this, boa2]() {
			boa2->getMngr()->setActive(boa2, false);
			roomEvent[Boa2]();
			});*/
		//Test obj 3
		/*auto hanni = entityFactory->CreateInteractableEntity(entityManager, "Hanni", EntityFactory::RECTAREA, Vector2D(275 - 549 - 6, 700), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(hanni));
		entityManager->getComponent<ClickComponent>(hanni)->connect(ClickComponent::JUST_CLICKED, [this, hanni]() {
			hanni->getMngr()->setActive(hanni, false);
			roomEvent[Hanni]();
			});*/

		//Spoon
		auto spoon = entityFactory->CreateInteractableEntity(entityManager, "SceneSpoon", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 540), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(spoon));
		entityManager->getComponent<ClickComponent>(spoon)->connect(ClickComponent::JUST_CLICKED, [this, spoon]() {
			spoon->getMngr()->setActive(spoon, false);
			roomEvent[Spoon]();
			});

		//Test obj 4
		/*auto doku = entityFactory->CreateInteractableEntity(entityManager, "AAA", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 340), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(doku));
		entityManager->getComponent<ClickComponent>(doku)->connect(ClickComponent::JUST_CLICKED, [this, doku]() {
			doku->getMngr()->setActive(doku, false);
			roomEvent[Doku]();
			});*/


		// Button that confirms the assesination
		auto buttonPosible = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3) - 20), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonPosible)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[GoodEnd]();});
		entityManager->setActive(buttonPosible,false);

		// Button that confirms the assesination is not possible
		auto buttonImposible = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3)), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonImposible)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[BadEnd]();});
		entityManager->setActive(buttonImposible, false);

		//Resolve the case
		roomEvent[ResolveBottons] = [this, buttonPosible, buttonImposible]() {
			entityManager->setActive(buttonPosible, true);
			entityManager->setActive(buttonImposible, true);
			};

		//X Button "B1"

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, _eventToRead);

		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);

		std::cout << finishallpuzzles << std::endl;
	

	}
	SDL_Delay(1000);

	
}



void Room1Scene::resolvedPuzzle(int i)
{
	if (i <4) {
		int auxevent = event_size;
		if (i == 0)  auxevent = PipePuzzleRsv;
		else if (i == 1)  auxevent = BooksPuzzleRsv;
		else if (i == 2)  auxevent = ClockPuzzleRsv;
		else if (i == 3)  auxevent = TeaCupPuzzleRsv;
		roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
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
