#include "TeaCupPuzzleScene.h"

#include "EntityFactory.h"
#include "../src/utils/Vector2D.h"
#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/DragComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/Image.h"
#include "../src/sdlutils/VirtualTimer.h"
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

using namespace std;

TeaCupPuzzleScene::TeaCupPuzzleScene()
{
	_spoonIsInCup = false;
	_poisonIsChecked = false;

	dialogueManager = new DialogueManager(1);
}

TeaCupPuzzleScene::~TeaCupPuzzleScene()
{

}

void TeaCupPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		
		isStarted = true;
		room = sr;
		
		dialogueManager->setScene(this);

		teaCupBackground = entityFactory->CreateImageEntity(entityManager, "TeaCupBackgroundWithoutSpoon", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		teaCupBackground->getMngr()->removeComponent<Area2D>(teaCupBackground);

		/*ecs::entity_t teaCupSpoon = entityFactory->CreateInteractableEntity( // Spoon entity
			entityManager, "TeaCupSpoon", EntityFactory::RECTAREA,
			Vector2D(100, 400), Vector2D(), 600, 400, 0,
			areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT);*/

		ecs::entity_t teaCup = entityFactory->CreateInteractableEntity( // Cup entity
			entityManager, "clockShape", EntityFactory::RECTAREA,
			Vector2D(400, 100), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		/*entityManager->addComponent<TriggerComponent>(teaCup) // Spoon enters the cup Area2D
			->connect(TriggerComponent::AREA_ENTERED, [teaCupBackground, this]()
				{
					_spoonIsInCup = true;
				});*/

		teaCup->getMngr()->removeComponent<Image>(teaCup);

		teaCup->getMngr()->getComponent<TriggerComponent>(teaCup)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			std::cout << "pasa por el triger de la taza" << std::endl;
			});
		//Assigns the trigger bolean to false
		teaCup->getMngr()->getComponent<TriggerComponent>(teaCup)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		//create the spoon
		//spoon = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, Vector2D(100, 400), Vector2D(0, 0), 600, 400, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT);


		entityManager->getComponent<ClickComponent>(teaCup) // The cup is clicked after introducing the spoon
			->connect(ClickComponent::JUST_CLICKED, [teaCup, this]()
				{
					if (_spoonIsInCup == false) return;
					_poisonIsChecked = true;

					// ... Change image revealing poinson or whatever  <-- TODO
					Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithPoison");
					teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
					startDialogue("PuzzleTaza2");
				});

		/*entityManager->getComponent<TriggerComponent>(teaCup) // Spoon leaves the cup Area2D
			->connect(TriggerComponent::AREA_LEFT, [this]()
				{
					_spoonIsInCup = false;
				});*/

		/*entityManager->getComponent<DragComponent>(teaCupSpoon) // The spoon is dropped in the cup
			->connect(DragComponent::DRAG_END, [teaCupBackground, teaCupSpoon, this]()
				{
					if (_spoonIsInCup == false) return;
					entityManager->setActive(teaCupSpoon, false);

					// ... Change image to cup with spoon  <-- TODO			
					Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithSpoon");
					teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
				});*/

		/*if (_spoonIsInCup) {
			//entityManager->setActive(spoon, false);
			//Change image to cup with spoon < --TODO
				Texture * tx = &sdlutils().images().at("TeaCupBackgroundWithSpoon");
			teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);

		}*/

		//BackButton
		//ENTIDADSINENTITYFACTORY
		auto _backButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(_backButton);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, []()
			{
				Game::Instance()->getSceneManager()->popScene();
			});

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

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");

		startDialogue("PuzzleTaza1");
	}

	createInvEntities(sr);
}

void TeaCupPuzzleScene::refresh()
{

}

void TeaCupPuzzleScene::unload()
{

}

bool TeaCupPuzzleScene::Check()
{
	return _poisonIsChecked;
}

bool TeaCupPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "TeaCupSpoon") {
		_spoonIsInCup = true; //???
		Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithSpoon");
		teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
		//spoon->getMngr()->setActive(spoon, true); 
		return true;
	}
	return false;
}
