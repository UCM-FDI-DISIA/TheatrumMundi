#include "TombPuzzleScene.h"
#include "ClickComponent.h"
#include "SceneRoomTemplate.h"
#include "Log.h"
#include "DialogueManager.h"
#include "Image.h"
#include "Transform.h"
TombPuzzleScene::TombPuzzleScene()
{
	dialogueManager = new DialogueManager(3);
}

TombPuzzleScene::~TombPuzzleScene()
{
}

void TombPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		XOpuzzleResolve = false;
		dragPuzzleResolve = false;
		room = sr;
		dialogueManager->setScene(this);
#pragma region UI

#pragma region Inventory

		//INVENTORY
		createInventoryUI();
		//Log


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
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
		if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA1_2P");
		else {
			startDialogue("TUMBA1_1P");
		}

#pragma endregion
#pragma region BackGround

		auto TombBackGround = entityFactory->CreateImageEntity(entityManager, "FondoPuzzleTumba", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region Entities

		//Left Chain, gives access to the XOPuzzle
		auto XOAccess = entityFactory->CreateInteractableEntity(entityManager, "Cadena", EntityFactory::RECTAREA, Vector2D(100, 250), Vector2D(0,0), 128, 128, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<ClickComponent>(XOAccess)->connect(ClickComponent::JUST_CLICKED, [this, _backButton]()
			{
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				room->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->loadScene(XO_PUZZLE, room);
			});

		//Right Chain, gives access to the DragPuzzle
		auto dragAccess = entityFactory->CreateInteractableEntity(entityManager, "Cadena", EntityFactory::RECTAREA, Vector2D(1000,250), Vector2D(0, 0), 128, 128, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<ClickComponent>(dragAccess)->connect(ClickComponent::JUST_CLICKED, [this, _backButton]()
			{
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				room->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->loadScene(DRAG_PUZZLE, room);
			});

		//Lock in the middle of the scene
		auto lock = entityFactory->CreateInteractableEntity(entityManager, "Candado", EntityFactory::RECTAREA, Vector2D(550, 300), Vector2D(0, 0), 259, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<ClickComponent>(lock)->connect(ClickComponent::JUST_CLICKED, [this,lock]()
			{
				if (Check()) { //If the puzzle is resolved 
					auto jewel = entityFactory->CreateInteractableEntity(entityManager, "Joya", EntityFactory::RECTAREA, Vector2D(600, 300), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
					entityManager->getComponent<ClickComponent>(jewel)->connect(ClickComponent::JUST_CLICKED, [this,jewel]()
						{
							AddInvItem("Joya", sdlutils().invDescriptions().at("Joya"), room->GetInventory()->setPosition(), room);
							entityManager->setActive(jewel,false);
						});

					auto shovel = entityFactory->CreateInteractableEntity(entityManager, "Pala", EntityFactory::RECTAREA, Vector2D(500, 300), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
					entityManager->getComponent<ClickComponent>(shovel)->connect(ClickComponent::JUST_CLICKED, [this, shovel]()
						{
							AddInvItem("Pala", sdlutils().invDescriptions().at("Pala"), room->GetInventory()->setPosition(), room);
							entityManager->setActive(shovel, false);
						});
					lock->getMngr()->setActive(lock, false);
					Win();
				}
			});
#pragma endregion


	}
	createInvEntities(sr);
}

bool TombPuzzleScene::Check()
{
    return XOpuzzleResolve && dragPuzzleResolve;
}

void TombPuzzleScene::Win()
{
	room->resolvedPuzzle(0);
	setSolved(true);
}

void TombPuzzleScene::refresh()
{
	if (startDg) {
		switch (numDialog)
		{
		case 1:
			if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA2_2P");
			else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA2_1P");
			startDg = false;
			break;
		case 2:
			if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA3_2P");
			else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA3_1P");
			startDg = false;
			break;

		default:
			break;
		}
	}
}
