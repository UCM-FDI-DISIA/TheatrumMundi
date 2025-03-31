#include "ClockPuzzleScene.h"

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
#include "../src/game/Game.h"
#include "Log.h"

#include "SceneRoomTemplate.h"

ClockPuzzleScene::ClockPuzzleScene() : ScenePuzzleTemplate()
{
	_actualHour = 90;
	_actualMinute = 0;
	hasLongClockHand = false;
	hasShortClockHand = false;
	dialogueManager = new DialogueManager(1);

}

ClockPuzzleScene::~ClockPuzzleScene()
{
}

void ClockPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {


		isStarted = true;
		//Register scene in dialogue manager
		dialogueManager->setScene(this);

		
		

		room = sr;
		AudioManager& a = AudioManager::Instance();
		Sound clockMinSound = sdlutils().soundEffects().at("aguja_minutero");
		Sound clockHorSound = sdlutils().soundEffects().at("aguja_horario");
		a.setVolume(clockMinSound, 0.2);
		a.setVolume(clockHorSound, 0.2);

		//create the clock
		clock = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(600, 300), Vector2D(0, 0), 200, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->addComponent<TriggerComponent>(clock);
		//Assigns the trigger bolean to true
		clock->getMngr()->getComponent<TriggerComponent>(clock)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		clock->getMngr()->getComponent<TriggerComponent>(clock)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		//create the clock hands : minute
		longClockHand = entityFactory->CreateInteractableEntity(entityManager, "minutero", EntityFactory::RECTAREA, Vector2D(680, 360), Vector2D(0, 0), 20, 70, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockMinTransform = longClockHand->getMngr()->getComponent<Transform>(longClockHand);
		if (!hasLongClockHand) longClockHand->getMngr()->setActive(longClockHand, false);

		//create the clock hands : hour
		shortClockHand = entityFactory->CreateInteractableEntity(entityManager, "horaria", EntityFactory::RECTAREA, Vector2D(695, 360), Vector2D(0, 0), 20, 60, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockHorTransform = shortClockHand->getMngr()->getComponent<Transform>(shortClockHand);
		if (!hasShortClockHand) shortClockHand->getMngr()->setActive(shortClockHand, false);

		//create the buttons: min
		auto _buttonMin = entityManager->addEntity();
		auto _buttonMinTransform = entityManager->addComponent<Transform>(_buttonMin, Vector2D(200, 600), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonMin, &sdlutils().images().at("clockMinButton"));

		entityManager->addComponent<RectArea2D>(_buttonMin, areaLayerManager);

		_actualMinute;

		ClickComponent* clockMinClick = entityManager->addComponent<ClickComponent>(_buttonMin);
		clockMinClick->connect(ClickComponent::JUST_CLICKED, [_clockMinTransform, clockMinSound, this]()
			{
				if (!getSolved()) {


					#ifdef DEBUG
					std::cout << "CLICKED MINUTERO\n";
					#endif // DEBUG
					if (hasLongClockHand) {
						AudioManager::Instance().playSound(clockMinSound);
						_clockMinTransform->setRot(_clockMinTransform->getRot() + 15);
						_actualMinute += 15;
						if (_actualMinute == 360) _actualMinute = 0;
					}
				}
			});


		//create the buttons: hor
		auto _buttonHor = entityManager->addEntity();
		auto _buttonHorTransform = entityManager->addComponent<Transform>(_buttonHor, Vector2D(200, 550), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonHor, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonHor, areaLayerManager);


		ClickComponent* clockHorClick = entityManager->addComponent<ClickComponent>(_buttonHor);
		clockHorClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform, clockHorSound, this]()
			{
				if (!getSolved()) {

#ifdef DEBUG
					std::cout << "CLICKED HORARIO\n";
#endif // DEBUG
					if (hasShortClockHand) {
						AudioManager::Instance().playSound(clockHorSound);
						_clockHorTransform->setRot(_clockHorTransform->getRot() + 30);
						_actualHour += 30;
						if (_actualHour == 360) _actualHour = 0;
					}
				}
			});



		//create the buttons: reset button
		auto _buttonResetPuzzle = entityManager->addEntity();
		auto _buttonRessetPuzzleTransform =
			entityManager->addComponent<Transform>(_buttonResetPuzzle, Vector2D(1200, 150), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonResetPuzzle, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonResetPuzzle);


		ClickComponent* clockResetClick = entityManager->addComponent<ClickComponent>(_buttonResetPuzzle);
		clockResetClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform, _clockMinTransform, this]()
			{
				if (!getSolved()) {

#ifdef DEBUG
					std::cout << "WAAAAAAAAAA\n";
#endif // DEBUG

					_clockHorTransform->setRot(90);
					_actualHour = 90;
					_clockMinTransform->setRot(0);
					_actualMinute = 0;
				}
			});

		//create the buttons: check
		auto _buttonCheck = entityManager->addEntity();
		auto _buttonCheckTransform = entityManager->addComponent<Transform>(_buttonCheck, Vector2D(100, 550), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonCheck, &sdlutils().images().at("clockCheckButton"));

		entityManager->addComponent<RectArea2D>(_buttonCheck, areaLayerManager);

		//room variant logic
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);
		entity_t container;
		if (variant <= 1) {
			container = entityFactory->CreateInteractableEntity(entityManager, "frascoV2", EntityFactory::RECTAREA,
				Vector2D(510, 548), Vector2D(0, 0), 340, 200, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			container->getMngr()->setActive(container, false);
		}
		else if (variant == 2) {
			container = entityFactory->CreateInteractableEntity(entityManager, "frascoV1", EntityFactory::RECTAREA,
				Vector2D(510, 548), Vector2D(0, 0), 340, 200, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			container->getMngr()->setActive(container, false);
		}
		ClickComponent* clockCheckClick = entityManager->addComponent<ClickComponent>(_buttonCheck);
		clockCheckClick->connect(ClickComponent::JUST_CLICKED, [variant,_buttonCheckTransform, sr, this,_buttonCheck,_buttonHor,_buttonMin, _buttonResetPuzzle,container]()
			{
				if (Check() && getSolved()) {
					container->getMngr()->setActive(container, true);

					Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
					//Assign to this inventory the hint;
					if(variant <=1)AddInvItem("frascoV2", "Un frasco vacio.",position, sr);
					else if (variant ==2)AddInvItem("frascoV1", "Contiene restos de algo.", position, sr);
#ifdef DEBUG
					std::cout << "wii";
#endif // DEBUG
					Win();

				}
			});

		ClickComponent* clk = entityManager->getComponent<ClickComponent>(container);
		clk->connect(ClickComponent::JUST_CLICKED, [this, container, sr]() {

			Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
			AddInvItem("frascoV1", "Me lo puedo beber??", position, sr);
			container->getMngr()->setActive(container, false);
			});

		//BackButton
		auto _backButton = entityManager->addEntity(ecs::grp::UI);
		entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(_backButton, areaLayerManager);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [sr]()
			{
				Game::Instance()->getSceneManager()->popScene();
			});
		

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1150, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
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

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->getPos().setX(925);
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
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->getPos().setX(60 + 268 / 3);

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
		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);
	}

	startDialogue("PuzzleReloj");

	//IMPORTANT this need to be out of the isstarted!!!
	createInvEntities(sr);
}


void ClockPuzzleScene::refresh()
{
}

void ClockPuzzleScene::unload()
{
}

/// <summary>
/// When the item is not Drag anymore checks if the item was drop into the cloack, if the answer is yes assigns the boolean of the cloack hand 
/// </summary>
/// <param name="itemId"></param> --> item to got dropped
/// <returns></returns> --> true if the item is a cloack hand and the cloack detected and false in other case
bool ClockPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "minutero") {
		hasLongClockHand = true;
		longClockHand->getMngr()->setActive(longClockHand, true);
		return true;
	}
	else if (itemId == "horaria") {
		hasShortClockHand = true;
		shortClockHand->getMngr()->setActive(shortClockHand, true);
		return true;
	}
	return false;
}

bool ClockPuzzleScene::Check()
{
	if (_actualHour == 180 && _actualMinute == 180&&!solved)
	{
		
		solved = true;
		return true;
	}
	else 
	return false;
}

void ClockPuzzleScene::Win()
{
	room->resolvedPuzzle(2);
	//setSolved(true);
}

