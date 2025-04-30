#include "TelePuzzleScene.h"

#include "EntityFactory.h"
#include "../src/utils/Vector2D.h"
#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/DragComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/Image.h"

#include "../src/sdlutils/SDLUtils.h"

#include "../src/components/Transform.h"

#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/Components/WriteTextComponent.h"

#include "AudioManager.h"

#include "SceneRoomTemplate.h"

#include <string>

#include "../../TheatrumMundi/DescriptionInfo.h"

#include "DialogueManager.h"


#include "GameSave.h"

using namespace std;

TelePuzzleScene::TelePuzzleScene()
{
	_spoonIsInCup = false;
	_poisonIsChecked = false;
	dialogCount = 0;
	dialogueManager = new DialogueManager(-1);
}

TelePuzzleScene::~TelePuzzleScene()
{

}

void TelePuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {

		isStarted = true;
		room = sr;


		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		dialogueManager->setScene(this);

		dialogCount = 0;

		tvBackground = entityFactory->CreateImageEntity(entityManager, "TutorialZoom1", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		tvBackground->getMngr()->removeComponent<Area2D>(tvBackground);

		ecs::entity_t tv = entityFactory->CreateInteractableEntityNotMoveSprite( // Cup entity
			entityManager, "clockShape", EntityFactory::RECTAREA,
			Vector2D(400, 150), Vector2D(), 1000, 500, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

	

		tv->getMngr()->removeComponent<Image>(tv);

		tv->getMngr()->getComponent<TriggerComponent>(tv)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			std::cout << "pasa por el triger de la taza" << std::endl;
			});
		//Assigns the trigger bolean to false
		tv->getMngr()->getComponent<TriggerComponent>(tv)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		//create the spoon
	

		entityManager->getComponent<ClickComponent>(tv) // The cup is clicked after introducing the spoon
			->connect(ClickComponent::JUST_CLICKED, [tv, this]()
				{
					if (_spoonIsInCup == false) return;
					_poisonIsChecked = true;

					// ... Change image revealing poinson or whatever  <-- TODO
					//Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithPoison");
					//tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tx);
					//startDialogue("PuzzleTaza2");
				});


		//BackButton
		exitButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		entityManager->addComponent<Transform>(exitButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(exitButton, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(exitButton);

		entityManager->setActive(exitButton, false);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(exitButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				Game::Instance()->getSceneManager()->popScene();
			});

		//INVENTORY
		//Invntory Background
		InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
		entityManager->setActive(InventoryBackground, false);

		upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		//InventoryButton
		inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, buttonSound]() //Lamda function
			{
				AudioManager::Instance().playSound(buttonSound);
				sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

				// If the inventory is active, activate the items
				if (sr->GetInventory()->getActive()) {
					entityManager->setActive(InventoryBackground, true);

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);

					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(downButton, false);
					entityManager->setActive(upButton, false);
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], false);
					}
				}
			});

		ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(-1);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(1);
			});

		entityManager->setActive(inventoryButton, false);

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "");

		startDialogue("Tutorial4");

	}
	else {
		entityManager->setActive(inventoryButton, true);
		entityManager->setActive(exitButton, false);
		startDialogue("Tutorial7");
	}

	


	createInvEntities(sr);
}

void TelePuzzleScene::endDialogue() {
	switch (dialogCount) {
	case 0:
		dialogueManager->setdisplayOnProcess(false);

		entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);

		entityManager->setActive(exitButton, true);

		dialogCount += 1;
		//logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);

		//Game::Instance()->getLog()->addDialogueLineLog(" ", "PW: 3711");

		//entityManager->setActive(botonBack, true);

		break;
	case 1:
		dialogueManager->setdisplayOnProcess(false);

		entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
		break;
	}
}


void TelePuzzleScene::unload()
{

}

bool TelePuzzleScene::Check()
{
	return _poisonIsChecked;
}

bool TelePuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "antena") {

		entityManager->setActive(inventoryButton, false);
		entityManager->setActive(InventoryBackground, false);
		entityManager->setActive(upButton, false);
		entityManager->setActive(downButton, false);

		_spoonIsInCup = true;

		// AnimationFrames
		tvAnimationFrames.clear();
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom2"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom3"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom4"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomKei"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomSol"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomLucy"));
		tvAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		tvAnimationFrames.push_back(&sdlutils().images().at("FondoNegro"));
		tvAnimationFrames.push_back(&sdlutils().images().at("FondoNegro"));
		

		currentFrameIndex = 0;
		
		frameTimer.resetTime(); 
		//startDialogue("Tutorial8");
		isAnimating = true;
		// Change to first texture
		tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tvAnimationFrames[0]);


		

		GameSave save;
		save.setTutoCompleted(true);
		save.Write("savegame.dat");
		
		
		return true;
		
	}
	return false;
}
void TelePuzzleScene::refresh()
{

	
	if (isAnimating && frameTimer.currRealTime() > 400 && (currentFrameIndex==4 || currentFrameIndex == 6 || currentFrameIndex == 8)) // 0.4 sec
	{
		currentFrameIndex++;
		

		if (currentFrameIndex >= tvAnimationFrames.size()) {
			
			isAnimating = false;
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			Game::Instance()->reset();
			return;
		}

		
		tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tvAnimationFrames[currentFrameIndex]);

		
		frameTimer.resetTime();
	}
	else if (isAnimating && frameTimer.currRealTime() > 1000) {
		
		currentFrameIndex++;


		if (currentFrameIndex >= tvAnimationFrames.size()) {

			isAnimating = false;
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			Game::Instance()->reset();
			return;
		}


		tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tvAnimationFrames[currentFrameIndex]);


		frameTimer.resetTime();

	}
}
