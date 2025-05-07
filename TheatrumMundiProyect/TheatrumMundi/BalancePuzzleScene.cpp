#include "BalancePuzzleScene.h"

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

BalancePuzzleScene::BalancePuzzleScene()
{
	_featherIsInBalance = false;
	_hasFeather = false;

	dialogueManager = new DialogueManager(1);
}

BalancePuzzleScene::~BalancePuzzleScene()
{

}

void BalancePuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {

		isStarted = true;
		room = sr;

		AudioManager& a = AudioManager
			::Instance();
		shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		dialogueManager->setScene(this);

		balanceBackground = entityFactory->CreateImageEntity(entityManager, "BotonHoraria", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		balanceBackground->getMngr()->removeComponent<Area2D>(balanceBackground);

		/*ecs::entity_t teaCupSpoon = entityFactory->CreateInteractableEntity( // Spoon entity
			entityManager, "TeaCupSpoon", EntityFactory::RECTAREA,
			Vector2D(100, 400), Vector2D(), 600, 400, 0,
			areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT);*/

		auto balance = entityFactory->
			CreateImageEntity(entityManager, "balance", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);



		auto balanceArea = entityFactory->CreateInteractableEntity( // balance entity
			entityManager, "balance", EntityFactory::RECTAREA,
			Vector2D(400, 100), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		//balanceArea->getMngr()->removeComponent<Image>(balanceArea);

	auto heart = entityFactory->CreateImageEntity(entityManager, "heart", Vector2D(250, 100), Vector2D(0, 0), 460, 280, 0, ecs::grp::DEFAULT);


		auto featherReward = entityFactory->CreateInteractableEntity( // featherReawrd entity
			entityManager, "pluma", EntityFactory::RECTAREA,
			Vector2D(400, 300), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT);

		entityManager->getComponent<ClickComponent>(featherReward)->connect(ClickComponent::JUST_CLICKED, [this, featherReward, sr]() {
			featherReward->getMngr()->setActive(featherReward, false);
			Vector2D position = sr->GetInventory()->setPosition();
			AddInvItem("pluma", "Fue dificil cogerla.", position, sr);
			});

		feather = entityFactory->CreateInteractableEntity( // feather entity
			entityManager, "pluma", EntityFactory::RECTAREA,
			Vector2D(400, 300), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		if (!_hasFeather) feather->getMngr()->setActive(feather, false);

		balanceArea->getMngr()->getComponent<TriggerComponent>(balanceArea)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			std::cout << "pasa por el triger de la balanza" << std::endl;
			});


		//Assigns the trigger bolean to false
		balanceArea->getMngr()->getComponent<TriggerComponent>(balanceArea)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});



		//BackButton
		//ENTIDADCONENTITYFACTORY
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);


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

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);

					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(InventoryBackground, false);
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
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");

		//startDialogue("PuzzleTaza1");
	}
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
}

void BalancePuzzleScene::init()
{
}


void BalancePuzzleScene::unload()
{

}

bool BalancePuzzleScene::Check()
{

	return _featherIsInBalance;
}

bool BalancePuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "pluma") {
		_hasFeather = true;
		_featherIsInBalance = true;
		feather->getMngr()->setActive(feather, true);

		return true;
	}
	return false;
}
