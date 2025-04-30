#include "Box.h"
#include "Vector2D.h"
#include "SDLUtils.h"
#include "DialogueManager.h"
#include "DragComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "TiledAreaComponent.h"
#include "SceneRoomTemplate.h"
#include "Log.h"
#include <list>
#include "Area2D.h"
#include "RectArea2D.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "Image.h"

Box::Box()
{
	dialogueManager = new DialogueManager(2);
}

Box::~Box()
{
}

void Box::init(SceneRoomTemplate* sr)
{
	
		if (!isStarted) {

			isStarted = true;
			//Register scene in dialogue manager
			dialogueManager->setScene(this);
			
			room = sr;
			//create the buttons
			
			//3,5,8,14

			auto _button1 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button2 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button3 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button4 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button5 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button6 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button7 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button8 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button9 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button10 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button11 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button12 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button13 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button14 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button15 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button16 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);



#pragma region UI


/*#pragma region Inventory

			//INVENTORY
			//Invntory Background
			auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1150, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
			entityManager->setActive(InventoryBackground, false);

			auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
			entityManager->setActive(upButton, false);

			auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
			entityManager->setActive(downButton, false);

			
			//InventoryButton
			
			auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(60 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
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
			UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, upButton, sr]() {

				//AudioManager::Instance().playSound(buttonSound);
				sr->scrollInventory(-1);
				});

			ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
			DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, downButton, sr]() {

				//AudioManager::Instance().playSound(buttonSound);
				sr->scrollInventory(1);
				});

			
			
			//Log




		//BackButton
			auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

			//Click component Open log button
			ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
			clkOpen->connect(ClickComponent::JUST_CLICKED, [this, inventoryButton, InventoryBackground, downButton, upButton, _backButton]()
				{
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
					HideInventoryItems(InventoryBackground, downButton, upButton, room);
					room->GetInventory()->setFirstItem(0);
					auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
					_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
					_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
					_backButtonImage->setPosOffset(0, 0);
					Game::Instance()->getSceneManager()->popScene();
				});

			//Log
			dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
			Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
			//startDialogue("Puerta");

#pragma endregion*/

		}
		//createInvEntities(sr);
	
}
