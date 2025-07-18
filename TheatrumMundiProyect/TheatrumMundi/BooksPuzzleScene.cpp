#include "BooksPuzzleScene.h"

#include "../src/utils/Vector2D.h";

#include "EntityFactory.h"

#include <array>

#include "../src/Components/ClickComponent.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/ecs/Manager.h"

#include "../src/components/Image.h"

#include <vector>

#include "iostream"

#include "../src/components/RectArea2D.h"

#include "../src/components/TriggerComponent.h"

#include "../src/Components/Transform.h"

#include "../src/Components/LogComponent.h"

#include "Room1.h"

#include "SceneRoomTemplate.h"

#include "DialogueManager.h"
#include "../src/game/Game.h"
#include "Log.h"
#include "InvAnimComponent.h"



using namespace std;

BooksPuzzleScene::BooksPuzzleScene()
{
	comb.resize(10);
	myComb.resize(3);

	for (int i = 0; i < comb.size(); ++i) {
		comb[i] = i;
	}
	for (int i = 0; i < myComb.size(); ++i) {
		myComb[i] = 0;
	}

	dialogueManager = new DialogueManager(1);
}

BooksPuzzleScene::~BooksPuzzleScene()
{
	
}

void BooksPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) 
	{
		room = sr;
		isStarted = true;
		//Register scene in dialogue manager
		dialogueManager->setScene(this);

		//Game::Instance()->getDialogueManager()->ReadDialogue(Puzzle2);
		//startDialogue("Puzzle2");

		//AudioManager& a = AudioManager::Instance();

		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		audioManager().setVolume(buttonSound, 0.2);

		std::shared_ptr<Sound> bookSound = sdlutils().soundEffects().at("libro");


		//INVENTORY
		createInventoryUI();

		//Puzzle Scene
		
		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "ShelfBackground1", Vector2D(0, 0), Vector2D(0, 0),1349, 748, 0, ecs::grp::DEFAULT);

		auto number1 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(555, 415), Vector2D(0, 0), /*109, 115*/ 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto number2 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(555, 455), Vector2D(0, 0), /*63, 127*/60, 40, 0, areaLayerManager, EntityFactory::NODRAG,ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto number3 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(555, 495), Vector2D(0, 0),/* 743, 280*/60, 40, 0, areaLayerManager, EntityFactory::NODRAG,ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		ClickComponent* clickNumber1 = entityManager->getComponent<ClickComponent>(number1);
		clickNumber1->connect(ClickComponent::JUST_CLICKED, [this, number1, buttonSound]() {
			
			audioManager().playSound(buttonSound);

			if (myComb[0] < 9) {
				myComb[0]++;
				cout << "NUM1: " << myComb[0] << endl;
			}
			else if (myComb[0] == 9) {
				myComb[0] = 0;
				cout << "NUM1: " << myComb[0] << endl;
			}
			number1->getMngr()->getComponent<Image>(number1)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[0])));
			});

		ClickComponent* clickNumber2 = entityManager->getComponent<ClickComponent>(number2);
		clickNumber2->connect(ClickComponent::JUST_CLICKED, [this, number2, buttonSound]() {
			
			audioManager().playSound(buttonSound);

			if (myComb[1] < 9) {
				myComb[1]++;
				cout << "NUM2: " << myComb[1] << endl;
			}
			else if (myComb[1] == 9) {
				myComb[1] = 0;
				cout << "NUM2: " << myComb[1] << endl;
			}
			number2->getMngr()->getComponent<Image>(number2)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[1])));
			});

		ClickComponent* clickNumber3 = entityManager->getComponent<ClickComponent>(number3);
		clickNumber3->connect(ClickComponent::JUST_CLICKED, [this, number3, buttonSound]() {
			
			audioManager().playSound(buttonSound);
			
			if (myComb[2] < 9) {
				myComb[2]++;
				cout << "NUM3: " << myComb[2] << endl;
			}
			else if (myComb[2] == 9) {
				myComb[2] = 0;
				cout << "NUM3: " << myComb[2] << endl;
			}
			number3->getMngr()->getComponent<Image>(number3)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[2])));
			});

		//REWARD ENTITY
		auto clock = entityFactory->CreateInteractableEntity(entityManager, "horaria", EntityFactory::RECTAREA,
			Vector2D(680, 600), Vector2D(0, 0), 150, 250, 90,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);
		clock->getMngr()->setActive(clock, false);	
		//variant logic
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);
		entity_t tag;
		if (variant <= 1) {
			tag = entityFactory->CreateInteractableEntity(entityManager, "etiquetaV1", EntityFactory::RECTAREA,
				Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			entityManager->setActive(tag, false);
		}
		 else if (variant == 2) {
			 tag = entityFactory->CreateInteractableEntity(entityManager, "etiquetaV2", EntityFactory::RECTAREA,
				 Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
				 areaLayerManager,
				 EntityFactory::NODRAG,
				 ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			 entityManager->setActive(tag, false);
		 }
		


		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(690,493), Vector2D(0, 0), 70, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, tag,clock,sr, StudyBackground,this, buttonSound]() {

			#ifdef DEBUG
				std::cout << "CLICK" << std::endl;
			#endif // DEBUG
			
			
			if (!solved && Check()) {
				Win();
				audioManager().playSound(sdlutils().soundEffects().at("OpenPuzzle"));
				Image* img = entityManager->getComponent<Image>(StudyBackground);
				img->setTexture(&sdlutils().images().at("estanteria2"));
				//std::cout << "WIN" << std::endl;
				clock->getMngr()->setActive(clock, true);
				tag->getMngr()->setActive(tag, true);
				
				clock->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, true);
				tag->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, true);
			}else audioManager().playSound(sdlutils().soundEffects().at("failOpenPuzzle"));
			});

		ClickComponent* clk = entityManager->getComponent<ClickComponent>(clock);
		clk->connect(ClickComponent::JUST_CLICKED, [this, clock, sr]() {

			Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
			AddInvItem("horaria", sdlutils().Instance()->invDescriptions().at("horaria"), position, sr);
			clock->getMngr()->setActive(clock, false);
			});

		clk = entityManager->getComponent<ClickComponent>(tag);
		clk->connect(ClickComponent::JUST_CLICKED, [variant,this, tag, sr]() {

			Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
			if (variant <= 1) {
				AddInvItem("etiquetaV1", sdlutils().Instance()->invDescriptions().at("etiquetaV1"), position, sr);
				startDialogue("RecogerVeneno1");
			}
			else if (variant == 2) {
				AddInvItem("etiquetaV2", sdlutils().Instance()->invDescriptions().at("etiquetaV2"), position, sr);
				startDialogue("RecogerVeneno2");
			}
			tag->getMngr()->setActive(tag, false);


			
			

			});





		//BOOKS
		auto backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20,20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK);
		backButton->getMngr()->setActive(backButton, false);

		 ButtonBookFirst = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook1", EntityFactory::RECTAREA, Vector2D(615, 0), Vector2D(0, 0), 109, 115, 0,areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		 ButtonBookSecond = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook2", EntityFactory::RECTAREA, Vector2D(536, 135), Vector2D(0, 0), 62, 127, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		 ButtonBookThird = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook3", EntityFactory::RECTAREA, Vector2D(743,280), Vector2D(0, 0), 69, 119, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto ImageBook = entityFactory->CreateImageEntity(entityManager, "bookA", Vector2D(100,100), Vector2D(0, 0), 1200, 600, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK);
		ImageBook->getMngr()->setActive(ImageBook, false);

		ClickComponent* ButtonBook1Click = entityManager->getComponent<ClickComponent>(ButtonBookFirst);
		ButtonBook1Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, backButton, checkButton, clock,tag, this, bookSound]() {
			
			audioManager().playSound(bookSound);
			
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookA"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			clock->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			tag->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* ButtonBook2Click = entityManager->getComponent<ClickComponent>(ButtonBookSecond);
		ButtonBook2Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, backButton, checkButton, clock, tag, this, bookSound]() {
			
			audioManager().playSound(bookSound);
			
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookB"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			clock->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			tag->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* ButtonBook3Click = entityManager->getComponent<ClickComponent>(ButtonBookThird);
		ButtonBook3Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, backButton, checkButton, clock, tag, this, bookSound]() {
			
			audioManager().playSound(bookSound);
			
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookC"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			clock->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			tag->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* clickbackButton = entityManager->getComponent<ClickComponent>(backButton);
		clickbackButton->connect(ClickComponent::JUST_CLICKED, [ImageBook, backButton, checkButton, clock, tag, this, buttonSound]() {
			
			audioManager().playSound(buttonSound);
			
			ImageBook->getMngr()->setActive(ImageBook, false);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, false);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, true);
			auto _backButtonImage = backButton->getMngr()->getComponent<Image>(backButton);
			_backButtonImage->setW(backButton->getMngr()->getComponent<Transform>(backButton)->getWidth());
			_backButtonImage->setH(backButton->getMngr()->getComponent<Transform>(backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);
			if (Check())
			{
				clock->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
				tag->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			}

			});

		// Put the dialog interaction area in front of the other interactables
		//areaLayerManager->sendFront(dialogInteractionArea->getLayerPos());

		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager,"B1",EntityFactory::RECTAREA,Vector2D(20,20),Vector2D(0,0),90,90,0,areaLayerManager,EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, _backButton, buttonSound]()
		{	
				audioManager().playSound(buttonSound);
			HideInventoryItems();
			sr->GetInventory()->setFirstItem(0);
			auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
			_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
			_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);
			Game::Instance()->getSceneManager()->popScene();
		});

		dialogueManager->Init(1, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		
	}
	
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);

	startDialogue("PuzzleLibros");
}

void BooksPuzzleScene::unload()
{
	
}

bool BooksPuzzleScene::Check()
{ //HERE WE PUT THE CORRECT COMBINATION : 6 - 4 - 1
	//CORRECT ACTUAL COMBINATION: 1 - 6 - 4

	if (myComb[0] == comb[1] &&
		myComb[1] == comb[6] &&
		myComb[2] == comb[4])
	{
		solved = true;
		return true;
	}
	else
	{
		return false;
	}

}

void BooksPuzzleScene::Win()
{
	entityManager->getComponent<ClickComponent>(ButtonBookFirst)->setActive(false);
	entityManager->getComponent<ClickComponent>(ButtonBookSecond)->setActive(false);
	entityManager->getComponent<ClickComponent>(ButtonBookThird)->setActive(false);
	room->resolvedPuzzle(1);
	//Game::Instance()->getSceneManager()->popScene();
}

void BooksPuzzleScene::ResolveScene()
{
	Win();
}


