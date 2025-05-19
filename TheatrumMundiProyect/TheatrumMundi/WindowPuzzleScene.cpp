#include "WindowPuzzleScene.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "DialogueManager.h"
#include "TriggerComponent.h"
#include "Image.h"
#include "Transform.h"
#include "Game.h"
#include "Log.h"

WindowPuzzleScene::WindowPuzzleScene()
{
	isStarted = false;
	dialogueManager =new DialogueManager(3);
}

WindowPuzzleScene::~WindowPuzzleScene()
{
}

void WindowPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{
		isStarted = true;
		room = sr;

#pragma region UI


#pragma region Inventory

		//INVENTORY
		//Invntory Background
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
		dialogueManager->setScene(this);
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		if (variant != 2) {
			if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("Ventana1_2P");
			else {
				startDialogue("Ventana1_1P");
			}
		}
		else {
			if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("Ventana2_2P");
			else {
				startDialogue("Ventana2_1P");
			}
			Win();
		}

#pragma endregion

#pragma region Background

		WindowBackGround = entityFactory->CreateImageEntity(entityManager, "FondoVentanaCerrada", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto window = entityFactory->CreateInteractableEntity(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(400, 60), Vector2D(0, 0), 559, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<TriggerComponent>(window)->setTargetGroup(ecs::grp::INVENTORY);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(window)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
		});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(window)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
		});

#pragma endregion

	}
	createInvEntities(sr,false);
}

bool WindowPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "VarillaInventario") {
		if(!isOpen)entityManager->getComponent<Image>(WindowBackGround)->setTexture(&sdlutils().images().at("FondoVentanaAbierta"));
		else entityManager->getComponent<Image>(WindowBackGround)->setTexture(&sdlutils().images().at("FondoVentanaCerrada"));
		isOpen = !isOpen;
		if(OpenCount == 0) Win();
		else if (OpenCount == 20) {
			HideInventoryItems();
			startDialogue("NoMePaganPorEsto");
		}
		++OpenCount;
		return true;
	}
	return false;
}

void WindowPuzzleScene::Win()
{
	room->resolvedPuzzle(5);
}
