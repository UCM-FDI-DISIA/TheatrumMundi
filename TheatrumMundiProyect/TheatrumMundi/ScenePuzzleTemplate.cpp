#include "ScenePuzzleTemplate.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DragComponent.h"
#include "TriggerComponent.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"
#include "SDLUtils.h"

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
		invObjects[i]->getMngr()->getComponent<Transform>(invObjects[i])->setPos(sr->GetInventory()->GetPosition(i));
	}

	/*for (int i = 0; i < sr->GetInventory()->hints.size(); ++i) {
		sr->GetInventory()->hints[i]->getMngr()->getComponent<Transform>(sr->GetInventory()->hints[i])->getPos().set(sr->GetInventory()->GetPosition(i));
	}*/
}

ScenePuzzleTemplate::ScenePuzzleTemplate(): SceneTemplate()
{
	placeHand = false;
}

ScenePuzzleTemplate::~ScenePuzzleTemplate()
{
	//if memory leak delete the vector of entities
}
/// <summary>
/// For all the Inventory itmes in the SceneRoomTemplate, we created a new Entity in the puzzle Scenes
/// </summary>
/// <param name="sr"></param> --> Referebce to thee SceneRoomTemplate
void ScenePuzzleTemplate::createInvEntities(SceneRoomTemplate* sr)
{
	//REMOVE INVALID ENTITIES
	compareInv(sr);
	//CREATE DESCRIPTION ENTITIES

	//REPOSITION THE INVENTORY ITEMS
	reposInv(sr);
	
	//visual background for item description text
	auto _backgroundTextDescription = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(150, 800), Vector2D(0, 0), 500, 75, 0, ecs::grp::DEFAULT);
	entityManager->setActive(_backgroundTextDescription, false);

	//description text entity
	auto textDescriptionEnt = entityManager->addEntity(ecs::grp::DEFAULT);
	auto _testTextTranform = entityManager->addComponent<Transform>(textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
	entityManager->setActive(textDescriptionEnt, false);
	SDL_Color colorDialog = { 255, 255, 255, 255 };
	entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, sr->GetInventory()->getTextDescription());


	int index = 0; // Need to search for the respective position of the item
	for (auto a : sr->GetInventory()->getItems()) {
		//if the array of names hasn't have the name of this entity then it means that is new and has to be created again
		if (!ItemAlreadyCreated(a->getID())) {

			//Add the item name to the array names

			invID.push_back(a->getID());
			//Add the entitie to the array
			invObjects.push_back(entityFactory->CreateInteractableEntity(entityManager, a->getID(), EntityFactory::RECTAREA, sr->GetInventory()->GetPosition(index), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT));

			auto it = invObjects.back();

			//Assign lamda functions

			//if you click in one item, assign the original position to the position of the object clicked
			it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this, sr, it, a, _backgroundTextDescription, textDescriptionEnt]() {
				setOriginalPos(it->getMngr()->getComponent<Transform>(it)->getPos());
				entityManager->setActive(_backgroundTextDescription, false);
				
				//hide item description when item has been clicked
				entityManager->setActive(textDescriptionEnt, false);
				});

			//if you drop the item, compares if it was drop in or out tge cloack
			it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this, sr, a, it, _backgroundTextDescription, textDescriptionEnt]() {
				//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
				if (!placeHand) it->getMngr()->getComponent<Transform>(it)->setPos(getOriginalPos());
				//in other case remove the item from this inventory and the inventory of Room1
				else {
					//Add the hand to the cloack
					if (isItemHand(a->getID())) {

						//remove the object and pos from the inventory
						sr->GetInventory()->removeItem(a->getID(), invObjects,invID);				
					}
					else it->getMngr()->getComponent<Transform>(it)->setPos(getOriginalPos());
				}

				entityManager->setActive(_backgroundTextDescription, false);

				//hide item description when item has been clicked
				entityManager->setActive(textDescriptionEnt, false);
				});

			//if mouse is on item, show item description
			it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_ENTERED, [this, sr, a, _backgroundTextDescription, textDescriptionEnt, it]() {
				if (!entityManager->getComponent<DragComponent>(it)->isBeingClicked())
				{
					//show item description entities
					entityManager->setActive(_backgroundTextDescription, true);
					entityManager->setActive(textDescriptionEnt, true);

					//change text description
					sr->GetInventory()->setTextDescription(a, entityManager->getComponent<Transform>(it), _backgroundTextDescription->getMngr()->getComponent<Transform>(_backgroundTextDescription));

				}
				
				
				
				});

			//if mouse leaves item, hide item description
			it->getMngr()->getComponent<TriggerComponent>(it)->connect(TriggerComponent::CURSOR_LEFT, [this, sr, a, _backgroundTextDescription, textDescriptionEnt]() {
				//hide item description entities
				entityManager->setActive(_backgroundTextDescription, false);
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
		sr->GetInventory()->addItem(new Hint(id, description, &sdlutils().images().at(id)));
		sr->GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(sr->GetEntityManager(), id, EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		sr->GetInventory()->hints.back()->getMngr()->setActive(sr->GetInventory()->hints.back(), false);
		invID.push_back(id);
		invObjects.push_back(entityFactory->CreateInteractableEntity(entityManager, id, EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT));
		auto it = invObjects.back();
		it->getMngr()->setActive(it, false);

		//Assign the lamda functions to the inventory item
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this,it]() {
			setOriginalPos(it->getMngr()->getComponent<Transform>(it)->getPos());
			});

		//if you drop the item, compares if it was drop in or out tge cloack
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this,id,sr,it]() {
			//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
			if (!placeHand) it->getMngr()->getComponent<Transform>(it)->setPos(getOriginalPos());
			//in other case remove the item from this inventory and the inventory of Room1
			else {
				//Add the hand to the cloack
				if (isItemHand(id)) {

					//remove the object from the inventory
					sr->GetInventory()->removeItem(id, invObjects, invID);
				}
				else it->getMngr()->getComponent<Transform>(it)->setPos(getOriginalPos());
			}
			});

		sr->createDescription(sr->GetInventory()->hints.back(), sr->GetInventory()->getItems().back());
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
				transform->setPosY(transform->getPos().getY() + 150); // Ajustar la posición de los objetos visibles
			}
			sr->GetInventory()->setFirstItem(sr->GetInventory()->getFirstItem() + 1);
			sr->scrollInventory(-1);
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
				transform->setPosY(transform->getPos().getY() - 150); // Ajustar la posición de los objetos visibles
			}
			sr->GetInventory()->setFirstItem(sr->GetInventory()->getFirstItem() - 1);
			sr->scrollInventory(1);
		}
	}
}

void ScenePuzzleTemplate::HideInventoryItems(const ecs::entity_t& InventoryBackground, const ecs::entity_t& downButton, const ecs::entity_t& upButton,SceneRoomTemplate* sr)
{
	sr->GetInventory()->setActive(false);
	entityManager->setActive(InventoryBackground, false);
	entityManager->setActive(downButton, false);
	entityManager->setActive(upButton, false);
	
	for (int i = 0; i < sr->GetInventory()->getItems().size(); ++i) {
		invObjects[i]->getMngr()->setActive(invObjects[i], false);
	}
}
