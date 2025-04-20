#include "WiresPuzzleScene.h"
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

using namespace std;

WiresPuzzleScene::WiresPuzzleScene() : lightsOn(0)
{
	
}

WiresPuzzleScene::~WiresPuzzleScene()
{

}

void WiresPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{
		isStarted = true;
		dialogueManager->setScene(this);

		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		//InventoryButton
		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton, buttonSound]() //Lamda function
			{
				AudioManager::Instance().playSound(buttonSound);
				sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

				// If the inventory is active, activate the items
				if (sr->GetInventory()->getActive()) {
					entityManager->setActive(InventoryBackground, true);
					entityManager->setActive(logbtn, false);

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);

					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}

				}
				else {
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(logbtn, true);
					entityManager->setActive(downButton, false);
					entityManager->setActive(upButton, false);
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], false);
					}
				}
			});

		ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, upButton, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(-1, sr);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton, sr]() {

			AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(1, sr);
		});

		//Puzzle scene
		room = sr;

		//where the wires are going to be connected
		ports[0] = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(415, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ports[1] = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(465, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ports[2] = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(515, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ports[3] = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(565, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ports[4] = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(615, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		//wires
		wires[0] = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook1", EntityFactory::RECTAREA, Vector2D(415, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		wires[1] = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook2", EntityFactory::RECTAREA, Vector2D(465, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		wires[2] = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook3", EntityFactory::RECTAREA, Vector2D(515, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		wires[3] = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook1", EntityFactory::RECTAREA, Vector2D(565, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		wires[4] = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook2", EntityFactory::RECTAREA, Vector2D(615, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		//lights
		lights[0] = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(300, 295), Vector2D(0, 0), 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[1] = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(300, 360), Vector2D(0, 0), 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[2] = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(300, 425), Vector2D(0, 0), 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[3] = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(300, 490), Vector2D(0, 0), 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[4] = entityFactory->CreateInteractableEntity(entityManager, "boa2", EntityFactory::RECTAREA, Vector2D(300, 555), Vector2D(0, 0), 60, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		//check the wires
		//trigger
		for (int i = 0; i < ports.size(); i++)
		{
			entityManager->addComponent<TriggerComponent>(ports[i]);

			ports[i]->getMngr()->getComponent<TriggerComponent>(ports[i])->connect(TriggerComponent::AREA_ENTERED, [this, i]() {
				auto portEntity = ports[i];
				auto portPos = entityManager->getComponent<Transform>(portEntity)->getPos();

				//look if the wire is in the same position as the port
				for (int j = 0; j < wires.size(); j++)
				{
					std::cout << "entra a segundo for" << std::endl;
					auto wireEntity = wires[j];
					auto wirePos = entityManager->getComponent<Transform>(wireEntity)->getPos();

					float distance = (wirePos - portPos).magnitude();
					if (distance < 20) {
						//if the wire is in the same position as the port, set the wire to the port position
						entityManager->getComponent<Transform>(wireEntity)->setPos(portPos);
						std::cout << "Wire " << j << " connected to port " << i << std::endl;
					}
					else
					{
						//if the wire is not in the same position as the port, set the wire to its original position
						entityManager->getComponent<Transform>(wireEntity)->setPos(wirePos);
					}
				}
				});
		}


		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(800, 493), Vector2D(0, 0), 70, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, sr, this, buttonSound]() {

			#ifdef DEBUG
			std::cout << "CLICK" << std::endl;
			#endif // DEBUG

			AudioManager::Instance().playSound(buttonSound);
			if (Check()) {
				Win();
				
			}
		});

		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, downButton, upButton, inventoryButton, _backButton, buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems(InventoryBackground, downButton, upButton, sr);
				sr->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->popScene();
			});

		dialogueManager->Init(1, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
	}
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
}

void WiresPuzzleScene::unload()
{

}

bool WiresPuzzleScene::Check()
{ 	//CORRECT ACTUAL COMBINATION: 3 - 1 - 5 - 2 - 4

	lightsOn = 0;
	for (int i = 0; i < actualPos.size(); i++)
	{
		if (actualPos[i] != -1) //verify that the wires are connected
		{
			if (actualPos[i] == winPos[i]) //verify that the wires are in the correct position
			{
				lightsOn++;
			}
		}
	}

	if (lightsOn == 5)
	{
		solved = true;
		return true;
	}
	else
	{
		return false;
	}
	
}

void WiresPuzzleScene::Win()
{
	
}

void WiresPuzzleScene::ResolveScene()
{

}