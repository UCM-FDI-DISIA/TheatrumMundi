#include "TelePuzzleScene.h"

#include "EntityFactory.h"
#include "../src/utils/Vector2D.h"
#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/DragComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/Image.h"
#include "InvAnimComponent.h"
#include "../src/sdlutils/SDLUtils.h"

#include "../src/components/Transform.h"

#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/Components/WriteTextComponent.h"

#include "AudioManager.h"

#include "SceneRoomTemplate.h"
#include "log.h"
#include <string>

#include "../../TheatrumMundi/DescriptionInfo.h"

#include "DialogueManager.h"

#include "../sdlutils/SoundEffect.h"
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
	//	AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");

		audioManager().setVolume(buttonSound, 0.2);

		dialogueManager->setScene(this);

		dialogCount = 0;

		tvBackground = entityFactory->CreateImageEntity(entityManager, "TutorialZoom1", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		tvBackground->getMngr()->removeComponent<Area2D>(tvBackground);

		ecs::entity_t tv = entityFactory->CreateInteractableEntityNotMoveSprite( // Cup entity
			entityManager, "clockShape", EntityFactory::RECTAREA,
			Vector2D(400, 150), Vector2D(), 1000, 500, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		tv->getMngr()->getComponent<TriggerComponent>(tv)->setTargetGroup(ecs::grp::INVENTORY);
	

		tv->getMngr()->removeComponent<Image>(tv);

		tv->getMngr()->getComponent<TriggerComponent>(tv)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		tv->getMngr()->getComponent<TriggerComponent>(tv)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});
	


		//BackButton
		exitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		entityManager->setActive(exitButton, false);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(exitButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [buttonSound]()
			{
				audioManager().playSound(buttonSound);
				Game::Instance()->getSceneManager()->popScene();
			});
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
		

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);
		entityManager->addComponent<InvAnimComponent>(InventoryBackground);
		invElems.push_back(InventoryBackground);
		auto upButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);
		invElems.push_back(upButton);
		auto downButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 640), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);
		invElems.push_back(downButton);
		//InventoryButton
		inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(129, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, InventoryBackground, upButton, downButton, buttonSound]() //Lamda function
			{
				audioManager().playSound(buttonSound);
				room->GetInventory()->setActive(!room->GetInventory()->getActive());  // Toggle the inventory

				// If the inventory is active, activate the items
				if (room->GetInventory()->getActive()) {
					entityManager->setActive(InventoryBackground, true);
					entityManager->getComponent<InvAnimComponent>(InventoryBackground)->startInvAnim();
					entityManager->setActive(logbtn, false);

					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);
					entityManager->getComponent<InvAnimComponent>(downButton)->startInvAnim();
					entityManager->getComponent<InvAnimComponent>(upButton)->startInvAnim();
					for (int i = room->GetInventory()->getFirstItem(); i < room->GetInventory()->getFirstItem() + room->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
						entityManager->getComponent<InvAnimComponent>(invObjects[i])->startInvAnim();
					}

				}
				else {
					entityManager->getComponent<InvAnimComponent>(InventoryBackground)->endInvAnim();
					entityManager->setActive(logbtn, true);
					entityManager->getComponent<InvAnimComponent>(downButton)->endInvAnim();
					entityManager->getComponent<InvAnimComponent>(upButton)->endInvAnim();

					for (int i = room->GetInventory()->getFirstItem(); i < room->GetInventory()->getFirstItem() + room->GetInventory()->getItemNumber(); ++i) {
						entityManager->getComponent<InvAnimComponent>(invObjects[i])->endInvAnim();
					}
				}
			});

		ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, upButton]() {

			audioManager().playSound(buttonSound);
			scrollInventoryPuzzle(-1, room);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton]() {

			audioManager().playSound(buttonSound);
			scrollInventoryPuzzle(1, room);
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


		entityManager->setActive(exitButton, true);

		dialogCount += 1;
		//logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);

		//Game::Instance()->getLog()->addDialogueLineLog(" ", "PW: 3711");

		//entityManager->setActive(botonBack, true);

		break;
	case 1:
		dialogueManager->setdisplayOnProcess(false);

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

		HideInventoryItems();
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
		audioManager().playSound(sdlutils().soundEffects().at("Statica"), true);
		audioManager().setVolume(sdlutils().soundEffects().at("Statica"), 0.1);

		

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
			
			Game::Instance()->setReset();
			return;
		}
		float vol = (float)currentFrameIndex * 0.1;
		audioManager().setVolume(sdlutils().soundEffects().at("Statica"), vol);
		
		tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tvAnimationFrames[currentFrameIndex]);

		
		frameTimer.resetTime();
	}
	else if (isAnimating && frameTimer.currRealTime() > 1000) {
		
		currentFrameIndex++;

		float vol = (float)currentFrameIndex * 0.1;
		audioManager().setVolume(sdlutils().soundEffects().at("Statica"), vol);

		if (currentFrameIndex >= tvAnimationFrames.size()) {

			isAnimating = false;
			
			Game::Instance()->setReset();
			return;
		}


		tvBackground->getMngr()->getComponent<Image>(tvBackground)->setTexture(tvAnimationFrames[currentFrameIndex]);


		frameTimer.resetTime();

	}
	if (currentFrameIndex == 10) {
		audioManager().stopSound(sdlutils().soundEffects().at("Statica"));
		audioManager().playSound(sdlutils().soundEffects().at("FinalTeleTutorial"));
	}
}
