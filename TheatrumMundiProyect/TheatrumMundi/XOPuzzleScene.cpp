#include "XOPuzzleScene.h"
#include "Log.h"
#include "SceneRoomTemplate.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "DialogueManager.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/LogComponent.h"
#include <cassert>
#include "AudioManager.h"


XOPuzzleScene::XOPuzzleScene(TombPuzzleScene* tomb)
{
	XO = { false,false,false,false,false };
	_tomb = tomb;
	dialogueManager = new DialogueManager(3);
}

XOPuzzleScene::~XOPuzzleScene()
{
}

void XOPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		//Register scene in dialogue manager
		dialogueManager->setScene(this);
		//startDialogue("PuzzleXO");
		room = sr;

		//create background
		entityFactory->CreateImageEntity(entityManager, "fondoXO", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);

		//create the buttons
		auto _button1 = entityFactory->CreateInteractableEntity(entityManager, "xButton", EntityFactory::RECTAREA, Vector2D(300, 300), Vector2D(0, 0), 183/1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _button2 = entityFactory->CreateInteractableEntity(entityManager, "xButton", EntityFactory::RECTAREA, Vector2D(450, 300), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _button3 = entityFactory->CreateInteractableEntity(entityManager, "xButton", EntityFactory::RECTAREA, Vector2D(600, 300), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _button4 = entityFactory->CreateInteractableEntity(entityManager, "xButton", EntityFactory::RECTAREA, Vector2D(750, 300), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _button5 = entityFactory->CreateInteractableEntity(entityManager, "xButton", EntityFactory::RECTAREA, Vector2D(900, 300), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _reset = entityFactory->CreateInteractableEntity(entityManager, "resetButton", EntityFactory::RECTAREA, Vector2D(1000, 500), Vector2D(0, 0), 183 / 1.5, 172 / 1.5, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ClickComponent* button1Click = entityManager->getComponent<ClickComponent>(_button1);
		button1Click->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				pushButton(0);
			});
		ClickComponent* button2Click = entityManager->getComponent<ClickComponent>(_button2);
		button2Click->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				pushButton(1);
			});
		ClickComponent* button3Click = entityManager->getComponent<ClickComponent>(_button3);
		button3Click->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				pushButton(2);
			});
		ClickComponent* button4Click = entityManager->getComponent<ClickComponent>(_button4);
		button4Click->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				pushButton(3);
			});
		ClickComponent* button5Click = entityManager->getComponent<ClickComponent>(_button5);
		button5Click->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				pushButton(4);
			});
		ClickComponent* _resetClick = entityManager->getComponent<ClickComponent>(_reset);
		_resetClick->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				resetButtons();
			});

		// Initial image
		Image* img1 = entityManager->getComponent<Image>(_button1);
		Image* img2 = entityManager->getComponent<Image>(_button2);
		Image* img3 = entityManager->getComponent<Image>(_button3);
		Image* img4 = entityManager->getComponent<Image>(_button4);
		Image* img5 = entityManager->getComponent<Image>(_button5);


		buttonImages = { img1, img2, img3, img4,img5 };
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
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
		//startDialogue("Puerta");

#pragma endregion
	}
	createInvEntities(sr);
}

void XOPuzzleScene::pushButton(int i)
{
	audioManager().stopSound(sdlutils().soundEffects().at("MecanismoNoAbre"));
	audioManager().playSound(sdlutils().soundEffects().at("MecanismoNoAbre"));
	if (i == 0) {
		XO[0] = !XO[0];
		XO[3] = !XO[3];
		updateButtonImage(0);
		updateButtonImage(3);
	}
	else if (i == 1) {
		XO[1] = !XO[1];
		XO[2] = !XO[2];
		updateButtonImage(1);
		updateButtonImage(2);
	}
	else if (i == 2) {
		XO[2] = !XO[2];
		XO[4] = !XO[4];
		updateButtonImage(2);
		updateButtonImage(4);
	}
	else if (i == 3) {
		XO[3] = !XO[3];
		//XO[1] = !XO[1];
		updateButtonImage(3);
		//updateButtonImage(1);
	}
	else if (i == 4) {
		XO[4] = !XO[4];
		XO[0] = !XO[0];
		updateButtonImage(4);
		updateButtonImage(0);
	}

	if (checkWin()) {
		Win();
		audioManager().playSound(sdlutils().soundEffects().at("MecanismoAbre"));
	}

}

void XOPuzzleScene::resetButtons()
{
	XO = { false,false,false,false,false };
	for (int i = 0; i < buttonImages.size(); i++) {
		updateButtonImage(i);
	}
}

void XOPuzzleScene::updateButtonImage(int index)
{
	if (XO[index])
		buttonImages[index]->setTexture(&sdlutils().images().at("oButton")); // Change to "O"
	else
		buttonImages[index]->setTexture(&sdlutils().images().at("xButton")); // Change to "X"
}

bool XOPuzzleScene::checkWin()
{
	bool win = true;
	int i = 0;

	while (win && i < XO.size()) {
		if (!XO[i]) win = false;
		i++;
	}

	return win;

}
void XOPuzzleScene::Win()
{
	if (!solved) {
		solved = true;
		_tomb->setXOpuzzle(true);
	}
}


