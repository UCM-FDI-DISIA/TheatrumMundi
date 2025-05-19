#include "ScenePuzzleTemplate.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DragComponent.h"
#include "TriggerComponent.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"
#include "SDLUtils.h"
#include "InvAnimComponent.h"
#include "../src/Components/WriteTextComponent.h"



void ScenePuzzleTemplate::Exit()
{
	Game::Instance()->getSceneManager()->popScene();
}


/// <summary>
/// Searcch if the item is already created
/// </summary>
/// <param name="id"></param> --> item to search
/// <returns></returns> --> return true if the item exists and false in the other case
bool ScenePuzzleTemplate::ItemAlreadyCreated(const std::string& id)
{
	for (auto a : invID) {
		if (a == id) return true;
	}
	return false;
}

void ScenePuzzleTemplate::compareInv(SceneRoomTemplate* sr)
{
	bool isItemInRoom = false;
	auto entityIt = invObjects.begin();
	auto IdIt = invID.begin();
	int index = 0;
	std::list<std::list<std::string>::iterator>IdToErase;
	for (auto id : invID) {
		for (auto srId : sr->GetInventory()->getItems()) {
			if (id == srId->getID()) {
				isItemInRoom = true;
				break;
			}
		}
	
		if (!isItemInRoom) {
			IdToErase.push_back(IdIt);
			invObjects.erase(entityIt);
			entityIt = invObjects.begin() + index;		
		}
		else {
			++entityIt;
			++index;
		}
		++IdIt;
		isItemInRoom = false;
	}
	while (!IdToErase.empty()) {
		invID.erase(IdToErase.front());
		IdToErase.pop_front();
	}
}

void ScenePuzzleTemplate::reposInv(SceneRoomTemplate* sr)
{
	for (int i = 0; i < invObjects.size(); ++i) {
		invObjects[i]->getMngr()->setActive(invObjects[i], false);
		invObjects[i]->getMngr()->getComponent<Transform>(invObjects[i])->setPos(sr->GetInventory()->GetPosition(i));
	}
}

void ScenePuzzleTemplate::CreateDescriptions(SceneRoomTemplate* sr)
{
	if (firstEntry) {

		//description text entity
		textDescriptionEnt = entityManager->addEntity(ecs::grp::UI);
		_testTextTranform = entityManager->addComponent<Transform>(textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
		entityManager->setActive(textDescriptionEnt, false);
		SDL_Color colorDialog = { 255, 255, 255, 255 };
		entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, sr->GetInventory()->getTextDescription());
		firstEntry = false;
	}
}

ScenePuzzleTemplate::ScenePuzzleTemplate(): SceneTemplate()
{
	placeHand = false;
	firstEntry = true;

}

ScenePuzzleTemplate::~ScenePuzzleTemplate()
{
	
}
void ScenePuzzleTemplate::createInventoryUI()
{
	std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
	AudioManager::Instance().setVolume(buttonSound, 0.2);

	
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
			AudioManager::Instance().playSound(buttonSound);
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

		AudioManager::Instance().playSound(buttonSound);
		scrollInventoryPuzzle(-1, room);
		});

	ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
	DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton]() {

		AudioManager::Instance().playSound(buttonSound);
		scrollInventoryPuzzle(1, room);
		});

}
/// <summary>
/// For all the Inventory itmes in the SceneRoomTemplate, we created a new Entity in the puzzle Scenes
/// </summary>
/// <param name="sr"></param> --> Referebce to thee SceneRoomTemplate
void ScenePuzzleTemplate::createInvEntities(SceneRoomTemplate* sr, bool remove)
{
	//REMOVE INVALID ENTITIES
	compareInv(sr);
	//CREATE DESCRIPTION ENTITIES ONLY FITST TIME
	CreateDescriptions(sr);
	//REPOSITION THE INVENTORY ITEMS
	reposInv(sr);
	
	int index = 0; // Need to search for the respective position of the item
	for (auto a : sr->GetInventory()->getItems()) {
		//if the array of names hasn't have the name of this entity then it means that is new and has to be created again
		if (!ItemAlreadyCreated(a->getID())) {

			//Add the item name to the array names

			invID.push_back(a->getID());
			//Add the entitie to the array
			invObjects.push_back(entityFactory->CreateInvEntity(entityManager, a->getID(), EntityFactory::RECTAREA, sr->GetInventory()->GetPosition(index), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::INVENTORY));

			auto it = invObjects.back();

			//Assign lamda functions

			//if you click in one item, assign the original position to the position of the object clicked
			it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this, sr, it, a]() {
				setOriginalPos(it->getMngr()->getComponent<Transform>(it)->getPos());
				
				//hide item description when item has been clicked
				entityManager->setActive(textDescriptionEnt, false);
				});

			//if you drop the item, compares if it was drop in or out tge cloack
			it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this, sr, a, it,remove]() {
				//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
				if (!placeHand) it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
				//in other case remove the item from this inventory and the inventory of Room1
				else {
					//Add the hand to the cloack
					if (isItemHand(a->getID())) {

						//remove the object and pos from the inventory
						if(remove)sr->GetInventory()->removeItem(a->getID(), invObjects,invID);	
						else  it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
					}
					else it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
				}

				//hide item description when item has been clicked
				entityManager->setActive(textDescriptionEnt, false);
				});

			//if mouse is on item, show item description
			it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_ENTERED, [this, sr, a, it]() {
				if (!entityManager->getComponent<DragComponent>(it)->isBeingClicked())
				{
					//show item description entities
					entityManager->setActive(textDescriptionEnt, true);

					//change text description
					sr->GetInventory()->setTextDescription(a, entityManager->getComponent<Transform>(it));

				}
				
				
				
				});

			//if mouse leaves item, hide item description
			it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_LEFT, [this, sr, a]() {
				//hide item description entities
				entityManager->setActive(textDescriptionEnt, false);
				});

			//Set the active item to false
			it->getMngr()->setActive(it, false);
		}
		++index; //Add 1 to the position index
	}
}

/// <summary>
/// Add a new item to the InventoryEntity Array
/// </summary>
/// <param name="id"></param> --> Name of the item to include
/// <param name="position"></param> --> Position of the new Item to set
/// <param name="sr"></param> --> Reference to the SceneRoom
void ScenePuzzleTemplate::AddInvItem(const std::string& id, const std::string& description, const Vector2D& position, SceneRoomTemplate* sr)
{

	if (!ItemAlreadyCreated(id)) {
		//Add Items to the room inventory
		sr->GetInventory()->addItem(new Hint(id, description, &sdlutils().images().at(id)));
		sr->GetInventory()->hints.push_back(entityFactory->CreateInvEntity(sr->GetEntityManager(), id, EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, sr->GetAreaLayerManager(), EntityFactory::NODRAG, ecs::grp::UI));
		sr->GetInventory()->hints.back()->getMngr()->setActive(sr->GetInventory()->hints.back(), false);

		auto srHint = sr->GetInventory()->hints.back();
		auto srItem = sr->GetInventory()->getItems().back();
		//description text entity
		entityManager->setActive(textDescriptionEnt, false);

		//Add to the puzzle Lists
		invID.push_back(id);
		invObjects.push_back(entityFactory->CreateInvEntity(entityManager, id, EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::INVENTORY));
		auto it = invObjects.back();
		auto item = sr->GetInventory()->getItems().back();

		//Entities Logic
		if(sr->GetInventory()->getActive() == true && sr->GetInventory()->getItems().size() < 4) it->getMngr()->setActive(it, true); //If inventory is showing and we have less than 2 items, set the AddedItem to True
		else it->getMngr()->setActive(it, false);

		//Assign the lamda functions to the inventory item
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this,it]() {
			setOriginalPos(it->getMngr()->getComponent<Transform>(it)->getPos());
			entityManager->setActive(textDescriptionEnt, false);
			});

		//if you drop the item, compares if it was drop in or out tge cloack
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this,id,sr,it]() {
			//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
			if (!placeHand) it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
			//in other case remove the item from this inventory and the inventory of Room1
			else {
				//Add the hand to the cloack
				if (isItemHand(id)) {
					//remove the object from the inventory
					if (remove)sr->GetInventory()->removeItem(id, invObjects, invID);
					else  it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
				}
				else it->getMngr()->getComponent<Transform>(it)->setPosPure(getOriginalPos());
			}
			entityManager->setActive(textDescriptionEnt, false);
			});

		//if mouse is on item, show item description
		it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_ENTERED, [this, sr,item, it]() {
			if (!entityManager->getComponent<DragComponent>(it)->isBeingClicked())
			{
				//show item description entities
				entityManager->setActive(textDescriptionEnt, true);
				//change text description
				sr->GetInventory()->setTextDescription(item, entityManager->getComponent<Transform>(it));

			}
		});
		//if mouse leaves item, hide item description
		it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_LEFT, [this, sr]() {
			//hide item description entities
			entityManager->setActive(textDescriptionEnt, false);
		});
		sr->createDescription(srHint,srItem);
	}
}

void ScenePuzzleTemplate::scrollInventoryPuzzle(int dir, SceneRoomTemplate* sr)
{
	if (!sr->GetInventory() || invObjects.empty()) return;

	if (dir == -1) { // Scroll UP
		if (sr->GetInventory()->getFirstItem() > 0) {

			int lastVisibleIndex = sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber() - 1;
			if (lastVisibleIndex < invObjects.size()) {
				invObjects[lastVisibleIndex]->getMngr()->setActive(invObjects[lastVisibleIndex], false);
			}


			sr->GetInventory()->setFirstItem(sr->GetInventory()->getFirstItem() - 1);


			invObjects[sr->GetInventory()->getFirstItem()]->getMngr()->setActive(invObjects[sr->GetInventory()->getFirstItem()], true);


			for (int i = 0; i < invObjects.size(); ++i) {
				auto transform = invObjects[i]->getMngr()->getComponent<Transform>(invObjects[i]);
				Vector2D auxVector = Vector2D(transform->getPos().getX(), transform->getPos().getY() + (150 * Game::Instance()->hscreenScale));
				transform->setPosPure(auxVector); // Ajustar la posición de los objetos visibles
			}
			//sr->scrollInventory(-1);
		}
	}
	else if (dir == 1) { // Scroll DOWN
		if (sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber() < invObjects.size()) {
			// Desactivar el primer objeto visible
			invObjects[sr->GetInventory()->getFirstItem()]->getMngr()->setActive(invObjects[sr->GetInventory()->getFirstItem()], false);
			sr->GetInventory()->setFirstItem(sr->GetInventory()->getFirstItem() + 1);

			int newLastVisibleIndex = sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber() - 1;
			if (newLastVisibleIndex < invObjects.size()) {
				// Activar el nuevo último objeto visible
				invObjects[newLastVisibleIndex]->getMngr()->setActive(invObjects[newLastVisibleIndex], true);
			}

			// Ajustar las posiciones de los objetos
			for (int i = 0; i < invObjects.size(); ++i) {
				auto transform = invObjects[i]->getMngr()->getComponent<Transform>(invObjects[i]);
				Vector2D auxVector = Vector2D(transform->getPos().getX(), transform->getPos().getY() - (150 * Game::Instance()->hscreenScale));
				transform->setPosPure(auxVector); // Ajustar la posición de los objetos visibles
			}
			//sr->scrollInventory(1);
		}
	}
}

void ScenePuzzleTemplate::HideInventoryItems()
{
	room->GetInventory()->setActive(false);
	room->reposAllInventoryItems();
	for (auto& a : invElems) entityManager->setActive(a,false);
	
	for (int i = 0; i < room->GetInventory()->getItems().size(); ++i) {
		invObjects[i]->getMngr()->setActive(invObjects[i], false);

	}
}
