#include "Box.h"
#include "Vector2D.h"
#include "SDLUtils.h"
#include "DialogueManager.h"
#include "DragComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "TiledAreaComponent.h"
#include "SceneRoomTemplate.h"
#include "Log.h"
#include <list>
#include "Area2D.h"
#include "RectArea2D.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "Image.h"




Box::Box()
{
	dialogueManager = new DialogueManager(2);
}

Box::~Box()
{
}

void Box::init(SceneRoomTemplate* sr)
{
	
		if (!isStarted) {

			isStarted = true;
			//Register scene in dialogue manager
			dialogueManager->setScene(this);

			AudioManager& a = AudioManager::Instance();
			std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
			a.setVolume(buttonSound, 0.2);
			
			room = sr;
			//create the buttons
			rmObjects.background = entityFactory->CreateImageEntity(entityManager, "cajaFuerteCerrada", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
			
			//3,5,8,14

			auto _button1 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button2 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button3 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button4 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 200), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button5 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button6 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button7 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button8 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 250), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button9 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button10 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button11 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button12 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 300), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			auto _button13 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button14 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(568, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button15 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(618, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
			auto _button16 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(668, 350), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			//add buttons to buttons' vector:
			rmObjects.puzzleButtons.push_back(_button1);
			rmObjects.puzzleButtons.push_back(_button2);
			rmObjects.puzzleButtons.push_back(_button3);
			rmObjects.puzzleButtons.push_back(_button4);

			rmObjects.puzzleButtons.push_back(_button5);
			rmObjects.puzzleButtons.push_back(_button6);
			rmObjects.puzzleButtons.push_back(_button7);
			rmObjects.puzzleButtons.push_back(_button8);

			rmObjects.puzzleButtons.push_back(_button9);
			rmObjects.puzzleButtons.push_back(_button10);
			rmObjects.puzzleButtons.push_back(_button11);
			rmObjects.puzzleButtons.push_back(_button12);

			rmObjects.puzzleButtons.push_back(_button13);
			rmObjects.puzzleButtons.push_back(_button14);
			rmObjects.puzzleButtons.push_back(_button15);
			rmObjects.puzzleButtons.push_back(_button16);
			
				ClickComponent* button1Click = entityManager->addComponent<ClickComponent>(_button1);
				button1Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(1);
					});
				ClickComponent* button2Click = entityManager->addComponent<ClickComponent>(_button2);
				button2Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(2);
					});
				ClickComponent* button3Click = entityManager->addComponent<ClickComponent>(_button3);
				button3Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(3);
					});
				ClickComponent* button4Click = entityManager->addComponent<ClickComponent>(_button4);
				button4Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(4);
					});
				ClickComponent* button5Click = entityManager->addComponent<ClickComponent>(_button5);
				button5Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(5);
					});
				ClickComponent* button6Click = entityManager->addComponent<ClickComponent>(_button6);
				button6Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(6);
					});
				ClickComponent* button7Click = entityManager->addComponent<ClickComponent>(_button7);
				button7Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(7);
					});
				ClickComponent* button8Click = entityManager->addComponent<ClickComponent>(_button8);
				button8Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(8);
					});
				ClickComponent* button9Click = entityManager->addComponent<ClickComponent>(_button9);
				button9Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(9);
					});
				ClickComponent* button10Click = entityManager->addComponent<ClickComponent>(_button10);
				button10Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(10);
					});
				ClickComponent* button11Click = entityManager->addComponent<ClickComponent>(_button11);
				button11Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(11);
					});
				ClickComponent* button12Click = entityManager->addComponent<ClickComponent>(_button12);
				button12Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(12);
					});
				ClickComponent* button13Click = entityManager->addComponent<ClickComponent>(_button13);
				button13Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(13);
					});
				ClickComponent* button14Click = entityManager->addComponent<ClickComponent>(_button14);
				button14Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(14);
					});
				ClickComponent* button15Click = entityManager->addComponent<ClickComponent>(_button15);
				button15Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(15);
					});
				ClickComponent* button16Click = entityManager->addComponent<ClickComponent>(_button16);
				button16Click->connect(ClickComponent::JUST_CLICKED, [this]()
					{
						pushButton(16);
					});
			
		
			// Initial image
			Image* img1 = entityManager->getComponent<Image>(_button1);
			Image* img2 = entityManager->getComponent<Image>(_button2);
			Image* img3 = entityManager->getComponent<Image>(_button3);
			Image* img4 = entityManager->getComponent<Image>(_button4);
			Image* img5 = entityManager->getComponent<Image>(_button5);
			Image* img6 = entityManager->getComponent<Image>(_button6);
			Image* img7 = entityManager->getComponent<Image>(_button7);
			Image* img8 = entityManager->getComponent<Image>(_button8);
			Image* img9 = entityManager->getComponent<Image>(_button9);
			Image* img10 = entityManager->getComponent<Image>(_button10);
			Image* img11 = entityManager->getComponent<Image>(_button11);
			Image* img12 = entityManager->getComponent<Image>(_button12);
			Image* img13 = entityManager->getComponent<Image>(_button13);
			Image* img14 = entityManager->getComponent<Image>(_button14);
			Image* img15 = entityManager->getComponent<Image>(_button15);
			Image* img16 = entityManager->getComponent<Image>(_button16);
			

			buttonImages = { img1, img2, img3, img4,img5,img6,img7,img8,img9,img10,img11,img12,img13,img14,img15,img16 };
			buttonImagesIni = { "bookComb0","bookComb0","bookComb0","bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0", "bookComb0"};
			//Change this when we have the final assets
			


#pragma region UI


#pragma region Inventory

		//INVENTORY
		createInventoryUI();

		//BackButton
			auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

			//Click component Open log button
			ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
			clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton, buttonSound]()
				{
					AudioManager::Instance().playSound(buttonSound);

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
			//Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
			//startDialogue("Puerta");

#pragma endregion
			int variant = Game::Instance()->getDataManager()->GetRoomVariant(2);
			
			flashlight = entityFactory->CreateInteractableEntity(entityManager, "Linterna", EntityFactory::RECTAREA,
				Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			
			ClickComponent* clk = entityManager->getComponent<ClickComponent>(flashlight);
			clk->connect(ClickComponent::JUST_CLICKED, [this, variant, sr]() {

				Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
				AddInvItem("Linterna", sdlutils().Instance()->invDescriptions().at("LuzVioleta"), position, sr);
				flashlight->getMngr()->setActive(flashlight, false);

				});
			entityManager->setActive(flashlight, false);

			if (variant == 1)knife = entityFactory->CreateInteractableEntity(entityManager, "CuchilloSerrado", EntityFactory::RECTAREA,
				Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			else knife = entityFactory->CreateInteractableEntity(entityManager, "CuchilloLiso", EntityFactory::RECTAREA,
				Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			clk = entityManager->getComponent<ClickComponent>(knife);
			clk->connect(ClickComponent::JUST_CLICKED, [this, variant, sr]() {

				Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
				if (variant == 1)AddInvItem("CuchilloSerrado", sdlutils().Instance()->invDescriptions().at("CuchilloDentado"), position, sr);
				
				else AddInvItem("CuchilloLiso", sdlutils().Instance()->invDescriptions().at("CuchilloFilo"), position, sr);
				knife->getMngr()->setActive(knife, false);

				});
			entityManager->setActive(knife, false);
		}
		sr->GetInventory()->setFirstItem(0);
		createInvEntities(sr);
	
}

void Box::pushButton(int i)
{
	if (!completed) {
		//buttonImages[i]->setTexture(&sdlutils().images().at("letterPushed")); // The one we have to use in the end
		buttonImages[i - 1]->setTexture(&sdlutils().images().at("bookComb1"));

		if (stage == 0) {
			if (i == 3) {
				lastWright = true;
			}
			else {
				lastWright = false;
			}
		}

		if (stage == 1) {
			if (i == 8) {
				lastWright = true;
			}
			else {
				lastWright = false;
			}
		}

		if (stage == 2) {
			if (i == 14) {
				lastWright = true;
			}
			else {
				lastWright = false;
			}
		}

		if (stage == 3) {
			if (i == 5) {
				lastWright = true;
			}
			else {
				lastWright = false;
			}
		}

		if (lastWright || stage < 1) {
			stage++;
			std::cout << "Stage: " << stage << std::endl;
			if (stage == 4) {
				Win();
				

			}
		}
		else {

			for (int j = 0; j < buttonImages.size(); j++) {

				buttonImages[j]->setTexture(&sdlutils().images().at(buttonImagesIni[j]));

			}
			stage = 0;
		}
	}
}

void Box::Win()
{
	rmObjects.background->getMngr()->getComponent<Image>(rmObjects.background)->setTexture(&sdlutils().images().at("cajaFuerteAbierta"));
	
	for (ecs::entity_t button : rmObjects.puzzleButtons)
	{
		entityManager->setActive(button, false);
	}

	completed = true;
	entityManager->setActive(flashlight, true);
	entityManager->setActive(knife, true);
	std::cout << "WIN" << std::endl;
}
