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

		raven = entityFactory->CreateImageEntity(entityManager, "CuervoLlave", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto DropSquare = entityFactory->CreateInteractableEntity(entityManager, "ChangeRoom",EntityFactory::RECTAREA, Vector2D(530, 150), Vector2D(0, 0), 300, 600, 0,areaLayerManager,EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<TriggerComponent>(DropSquare)->setTargetGroup(ecs::grp::INVENTORY);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(DropSquare)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(DropSquare)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		//Creation of the key and their logic
		key = entityFactory->CreateInteractableEntity(entityManager, "Llave", EntityFactory::RECTAREA, Vector2D(555, 590), Vector2D(0, 0), 855 / 3, 309 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		entityManager->getComponent<ClickComponent>(key)->connect(ClickComponent::JUST_CLICKED, [this, sr]()
			{
				if (ravenHappy) { //If you give the jewel to the bird, the key is pickable
					AudioManager::Instance().playSound(sdlutils().soundEffects().at("LlaveMoviendose"));
					Vector2D position = sr->GetInventory()->setPosition();
					AddInvItem("LlaveInv", sdlutils().invDescriptions().at("Llave"), position, sr);
					entityManager->removeEntity(key);
					Win();
				}
				//	else sound of angry bird
			});
		key->getMngr()->setActive(key,false);
		//Log
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
		if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("CUERVO1_2P");
		else startDialogue("CUERVO1_1P");
#pragma endregion
	}
	//IMPORTANT this need to be out of the isstarted!!!
	createInvEntities(sr);
}

bool RavenPuzzleScene::isItemHand(const std::string& itemId)
{
    if (itemId == "colganteInv") {
		ravenHappy = true;
		raven->getMngr()->getComponent<Image>(raven)->setTexture(&sdlutils().images().at("CuervoColgante"));
		key->getMngr()->setActive(key, true);
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

