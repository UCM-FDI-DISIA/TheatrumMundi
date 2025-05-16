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
}

WindowPuzzleScene::~WindowPuzzleScene()
{
}

void WindowPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{
		isStarted = true;
		hasRope = false;
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
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

		//startDialogue("Ventana");

#pragma endregion

#pragma region Background

		auto WindowBackGround = entityFactory->CreateImageEntity(entityManager, "FondoVentana", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::UNDER);

#pragma endregion

#pragma region SceneEntities

		auto window = entityFactory->CreateInteractableEntity(entityManager, "Ventana", EntityFactory::RECTAREA, Vector2D(550, 300), Vector2D(0, 0), 259, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->getComponent<TriggerComponent>(window)->setTargetGroup(ecs::grp::INVENTORY);
		//Assigns the trigger bolean to true
		entityManager->getComponent<TriggerComponent>(window)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
		});
		//Assigns the trigger bolean to false
		entityManager->getComponent<TriggerComponent>(window)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
		});
		rope = entityFactory->CreateImageEntity(entityManager, "cuerdaVentana", Vector2D(250, 200), Vector2D(0, 0), 259, 200, 0, ecs::grp::DEFAULT);
		rope->getMngr()->setActive(rope, false);
		openWindow = entityFactory->CreateImageEntity(entityManager, "VentanaAbierta", Vector2D(550, 300), Vector2D(0, 0), 259, 200, 0, ecs::grp::DEFAULT);
		openWindow->getMngr()->setActive(openWindow, false);

#pragma endregion

	}
	createInvEntities(sr);
}

bool WindowPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "CuerdaLarga") {

		rope->getMngr()->setActive(rope, true);
		hasRope = true;
		return true;
	}
	else if (itemId == "CuerdaCorta") {
		//Di�logo que dice que no se puede
		Win();
	}
	else if (itemId == "Gancho") {
		
		if (hasRope) {
			rope->getMngr()->setActive(rope, false);
			openWindow->getMngr()->setActive(openWindow, true);
			Win();
			return true;
		}
		else {
			//APLICAR SONIDO DE QUE NO SE LLEGA O IGUAL UN DIALOGO
			return false;
		}
		return false;
	}
	return false;
}

void WindowPuzzleScene::Win()
{
	room->resolvedPuzzle(5);
}
