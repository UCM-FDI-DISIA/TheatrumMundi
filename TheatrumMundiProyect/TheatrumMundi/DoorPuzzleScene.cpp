#include "DoorPuzzleScene.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DialogueManager.h"
#include "TriggerComponent.h"
#include "Game.h"
#include "Image.h"
#include "Transform.h"
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


#pragma region Inventory

		//INVENTORY
		createInventoryUI();
		//log


#pragma endregion

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

		//Log
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		//startDialogue("PuzzleCuervo");

#pragma endregion

#pragma region Background

		auto DoorBackGround = entityFactory->CreateImageEntity(entityManager, "PuertaSantuario", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto lock = entityFactory->CreateInteractableEntity(entityManager, "Candado", EntityFactory::RECTAREA, Vector2D(550, 300), Vector2D(0, 0), 259, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<TriggerComponent>(lock)->setTargetGroup(ecs::grp::INVENTORY);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(lock)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(lock)->connect(TriggerComponent::AREA_LEFT, [this]() {
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
	room->GetInventory()->setActive(false);
	room->GetInventory()->setFirstItem(0);
	room->resolvedPuzzle(2);
}
