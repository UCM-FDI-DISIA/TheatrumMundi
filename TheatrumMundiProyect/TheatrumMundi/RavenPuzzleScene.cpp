#include "RavenPuzzleScene.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DialogueManager.h"
#include "TriggerComponent.h"
#include "Transform.h"
#include "Game.h"
#include "Log.h"
#include "Image.h"
#include "Transform.h"
RavenPuzzleScene::RavenPuzzleScene()
{
	dialogueManager = new DialogueManager(3);
}

RavenPuzzleScene::~RavenPuzzleScene()
{
}

void RavenPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{
		isStarted = true;
		room = sr;
		ravenHappy = false;

#pragma region UI

#pragma region Inventory

		//INVENTORY
		createInventoryUI();
		//Log


#pragma endregion

		//Log
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		//startDialogue("PuzzleCuervo");
		dialogueManager->setScene(this);
		
		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton]()
			{
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				room->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->popScene();
			});


#pragma endregion

#pragma region Background

		auto RavenBackground = entityFactory->CreateImageEntity(entityManager, "FondoCuervo", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto raven = entityFactory->CreateInteractableEntity(entityManager, "CuervoPeligroso",EntityFactory::RECTAREA, Vector2D(200, 350), Vector2D(0, 0), 259, 200, 0,areaLayerManager,EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<TriggerComponent>(raven)->setTargetGroup(ecs::grp::INVENTORY);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(raven)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(raven)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		//Creation of the key and their logic
		auto key = entityFactory->CreateInteractableEntity(entityManager, "Llave", EntityFactory::RECTAREA, Vector2D(200, 400), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		entityManager->getComponent<ClickComponent>(key)->connect(ClickComponent::JUST_CLICKED, [this, sr,key]()
			{
				if (ravenHappy) { //If you give the jewel to the bird, the key is pickable
					AudioManager::Instance().playSound(sdlutils().soundEffects().at("LlaveMoviendose"));
					Vector2D position = sr->GetInventory()->setPosition();
					AddInvItem("Llave", sdlutils().invDescriptions().at("Llave"), position, sr);
					entityManager->setActive(key, false);
					Win();
				}
				//	else sound of angry bird
			});
		if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("CUERVO1_2P");
		else startDialogue("CUERVO1_1P");
#pragma endregion

	}
	//IMPORTANT this need to be out of the isstarted!!!
	
	createInvEntities(sr);
}

bool RavenPuzzleScene::isItemHand(const std::string& itemId)
{
    if (itemId == "Joya") {
		ravenHappy = true;
		AudioManager::Instance().playSound(sdlutils().soundEffects().at("cuervo"));
		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("CUERVO2_2P");
		else startDialogue("CUERVO2_1P");
		return true;
    }
	return false;
}

void RavenPuzzleScene::Win()
{
    room->resolvedPuzzle(1);
}

