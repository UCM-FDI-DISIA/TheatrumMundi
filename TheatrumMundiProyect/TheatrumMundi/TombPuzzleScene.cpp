#include "TombPuzzleScene.h"
#include "ClickComponent.h"
#include "SceneRoomTemplate.h"
#include "Log.h"
#include "Area2D.h"
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

#pragma region BackGround

		TombBackGround = entityFactory->CreateInteractableEntityNotMoveSprite(entityManager, "TumbaCerrada", EntityFactory::RECTAREA, Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UNDER);
		auto TombImage = entityManager->getComponent<Image>(TombBackGround);
		entityManager->getComponent<ClickComponent>(TombBackGround)->connect(ClickComponent::JUST_CLICKED, [this, TombImage]() {
			if (!open) {
				open = true;
				TombImage->setTexture(&sdlutils().images().at("TumbaPuzzles"));
				entityManager->setActive(dragAccess, true);
				entityManager->setActive(XOAccess, true);
				areaLayerManager->sendBack(TombBackGround->getMngr()->getComponent<Area2D>(TombBackGround)->getLayerPos());
			}
			});
#pragma endregion
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

#pragma endregion

#pragma region Entities

		//Left Chain, gives access to the XOPuzzle
		XOAccess = entityFactory->CreateInteractableEntity(entityManager, "Cadena", EntityFactory::RECTAREA, Vector2D(100, 250), Vector2D(0,0), 128, 128, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
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
		entityManager->setActive(XOAccess, false);

		//Right Chain, gives access to the DragPuzzle
		dragAccess = entityFactory->CreateInteractableEntity(entityManager, "Cadena", EntityFactory::RECTAREA, Vector2D(1000,250), Vector2D(0, 0), 128, 128, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
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
		entityManager->setActive(dragAccess, false);
#pragma endregion
		areaLayerManager->sendFront(TombBackGround->getMngr()->getComponent<Area2D>(TombBackGround)->getLayerPos());
		areaLayerManager->sendFront(_backButton->getMngr()->getComponent<Area2D>(_backButton)->getLayerPos());
		areaLayerManager->sendFront(inventoryButton->getMngr()->getComponent<Area2D>(inventoryButton)->getLayerPos());
	}
	dialogueManager->Init(2, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
	logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
	if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("TUMBA1_2P");
	else {
		startDialogue("TUMBA1_1P");
	}
	createInvEntities(sr);
}

bool TombPuzzleScene::Check()
{
    return XOpuzzleResolve && dragPuzzleResolve;
}

void TombPuzzleScene::Win()
{
	win = true;
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
			AudioManager::Instance().playSound(sdlutils().soundEffects().at("MoverPiedra"));
			startDg = false;
			break;

		default:
			break;
		}
	}
	if (!win && Check()) { //If the puzzle is resolved 
		auto neckclace = entityFactory->CreateInteractableEntity(entityManager, "ColganteTumba", EntityFactory::RECTAREA, Vector2D(600, 300), Vector2D(0, 0), 600, 600, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<ClickComponent>(neckclace)->connect(ClickComponent::JUST_CLICKED, [this, neckclace]()
			{
				AddInvItem("ColganteTumba", sdlutils().invDescriptions().at("ColganteTumba"), room->GetInventory()->setPosition(), room);
				entityManager->removeEntity(neckclace);
				entityManager->setActive(neckclace,false);
			});

		auto shovel = entityFactory->CreateInteractableEntity(entityManager, "Pala", EntityFactory::RECTAREA, Vector2D(500, 300), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<ClickComponent>(shovel)->connect(ClickComponent::JUST_CLICKED, [this, shovel]()
			{
				AddInvItem("Pala", sdlutils().invDescriptions().at("Pala"), room->GetInventory()->setPosition(), room);
				entityManager->removeEntity(shovel);
				entityManager->setActive(shovel,false);
			});
		XOAccess->getMngr()->setActive(XOAccess, false);
		dragAccess->getMngr()->setActive(dragAccess, false);
		XOAccess->getMngr()->removeEntity(XOAccess);
		dragAccess->getMngr()->removeEntity(dragAccess);

		auto TombImage = entityManager->getComponent<Image>(TombBackGround);
		TombImage->setTexture(&sdlutils().images().at("TumbaAbierta"));
		Win();
	}
}
