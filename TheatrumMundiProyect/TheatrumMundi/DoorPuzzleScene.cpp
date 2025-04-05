#include "DoorPuzzleScene.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DialogueManager.h"
#include "TriggerComponent.h"
#include "Game.h"
#include "Log.h"

DoorPuzzleScene::DoorPuzzleScene()
{
}

DoorPuzzleScene::~DoorPuzzleScene()
{
}

void DoorPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{
		isStarted = true;
		room = sr;

#pragma region UI


		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, []()
			{
				Game::Instance()->getSceneManager()->popScene();
			});

#pragma region Inventory

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1150, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		//InventoryButton
		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton]() //Lamda function
			{
				//AudioManager::Instance().playSound(buttonSound);
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
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ upButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(-1);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ downButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(1);
			});
		//Log


#pragma endregion

		//Log
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);

		//startDialogue("PuzzleCuervo");

#pragma endregion

#pragma region Background

		auto DoorBackGround = entityFactory->CreateImageEntity(entityManager, "PuertaSantuario", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto chain = entityFactory->CreateInteractableEntity(entityManager, "Candado", EntityFactory::RECTAREA, Vector2D(550, 300), Vector2D(0, 0), 259, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->addComponent<TriggerComponent>(chain);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(chain)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(chain)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

#pragma endregion

	}
	//IMPORTANT this need to be out of the isstarted!!!

	createInvEntities(sr);
}

bool DoorPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "Llave") {
		
		Win();
		//DoorSound, and maybe a Delay?
		Exit();
		return true;
	}
	return false;
}

void DoorPuzzleScene::Win()
{
	room->resolvedPuzzle(2);
}
