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
#include "ClickableSpriteComponent.h"

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


		//Create background
		auto background = entityFactory->CreateImageEntity(entityManager, "RelojFondo1",
			Vector2D((1349 - (4038 * std::min(1349.0 / 4038.0, 748.0 / 2244.0))) / 2.0,
				(748 - (2244 * std::min(1349.0 / 4038.0, 748.0 / 2244.0))) / 2.0),
			Vector2D(0, 0),
			4038 * std::min(1349.0 / 4038.0, 748.0 / 2244.0),
			2244 * std::min(1349.0 / 4038.0, 748.0 / 2244.0),
			0, ecs::grp::DEFAULT);

		//create the clock
		clock = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(420,50), Vector2D(0, 0), 300, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->addComponent<TriggerComponent>(clock);
		//Assigns the trigger bolean to true
		clock->getMngr()->getComponent<TriggerComponent>(clock)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			});
		//Assigns the trigger bolean to false
		clock->getMngr()->getComponent<TriggerComponent>(clock)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});
		entityManager->removeComponent<Image>(clock);


		//create the clock hands : minute
		longClockHand = entityFactory->CreateInteractableEntity(entityManager, "minutero", EntityFactory::RECTAREA, Vector2D(447, 23), Vector2D(0, 0), 150, 250, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockMinTransform = longClockHand->getMngr()->getComponent<Transform>(longClockHand);
		if (!hasLongClockHand) longClockHand->getMngr()->setActive(longClockHand, false);

		//create the clock hands : hour
		shortClockHand = entityFactory->CreateInteractableEntity(entityManager, "horaria", EntityFactory::RECTAREA, Vector2D(447,23), Vector2D(0, 0), 150, 250, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockHorTransform = shortClockHand->getMngr()->getComponent<Transform>(shortClockHand);
		if (!hasShortClockHand) shortClockHand->getMngr()->setActive(shortClockHand, false);

		//create the buttons: min
		//ENTIDADCONENTITYFACTORY
		auto _buttonMin = entityFactory->CreateInteractableEntity(entityManager, "BotonMinutero", EntityFactory::RECTAREA, Vector2D(400, 345), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		_actualMinute;

		ClickComponent* clockMinClick = entityManager->getComponent<ClickComponent>(_buttonMin);
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
		//ENTIDADCONENTITYFACTORY
		auto _buttonHor = entityFactory->CreateInteractableEntity(entityManager, "BotonHoraria", EntityFactory::RECTAREA, Vector2D(310, 345), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		ClickComponent* clockHorClick = entityManager->getComponent<ClickComponent>(_buttonHor);
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



		//ENTIDADCONENTITYFACTORY
		/*//create the buttons: reset button
		auto _buttonResetPuzzle = entityManager->addEntity();
		auto _buttonRessetPuzzleTransform =
			entityManager->addComponent<Transform>(_buttonResetPuzzle, Vector2D(1200, 150), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonResetPuzzle, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonResetPuzzle);


		ClickComponent* clockResetClick = entityManager->getComponent<ClickComponent>(_buttonResetPuzzle);
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
			*/
		//create the buttons: check
		//ENTIDADCONENTITYFACTORY
		auto _buttonCheck = entityFactory->CreateInteractableEntity(entityManager, "BotonComprobarReloj", EntityFactory::RECTAREA, Vector2D(600, 345), Vector2D(0, 0), 120, 120, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		auto _buttonCheckTransform = entityManager->getComponent<Transform>(_buttonCheck);

		//room variant logic
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);
		entity_t container;
		if (variant <= 1) {
			container = entityFactory->CreateInteractableEntity(entityManager, "frascoV2", EntityFactory::RECTAREA,
				Vector2D(500, 600), Vector2D(0, 0), 75, 125, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			container->getMngr()->setActive(container, false);
		}
		else if (variant == 2) {
			container = entityFactory->CreateInteractableEntity(entityManager, "frascoV1", EntityFactory::RECTAREA,
				Vector2D(500, 600), Vector2D(0, 0), 75, 125, 0,
				areaLayerManager,
				EntityFactory::NODRAG,
				ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
			container->getMngr()->setActive(container, false);
		}
		ClickComponent* clockCheckClick = entityManager->getComponent<ClickComponent>(_buttonCheck);
		clockCheckClick->connect(ClickComponent::JUST_CLICKED, [variant,_buttonCheckTransform, sr, this,_buttonCheck,_buttonHor,_buttonMin,container,background]()
			{
				if (Check() && getSolved()) {
					Image* img = entityManager->getComponent<Image>(background);
					img->setTexture(&sdlutils().images().at("FondoReloj2"));
					container->getMngr()->setActive(container, true);

					Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
					//Assign to this inventory the hint;
					if(variant <=1)AddInvItem("frascoV2", "Un frasco totalmente limpio, que raro.",position, sr);
					else if (variant ==2)AddInvItem("frascoV1", "Un frasco con restos de algo. Tiene huellas de dedos", position, sr);

#ifdef DEBUG
					std::cout << "wii";
#endif // DEBUG
					Win();

				}
			});

		//REWARD
		ClickComponent* clk = entityManager->getComponent<ClickComponent>(container);
		clk->connect(ClickComponent::JUST_CLICKED, [this, container, sr]() {

		
			container->getMngr()->setActive(container, false);
			});

		//BackButton
		//ENTIDADCONENTITYFACTORY
		entity_t _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		

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

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);

					for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
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
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ upButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(-1, sr);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ downButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			scrollInventoryPuzzle(1,sr);
			});

		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);
	
		startDialogue("PuzzleReloj");

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this,sr,InventoryBackground,downButton,upButton,inventoryButton, _backButton]()
			{
				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems(InventoryBackground, downButton, upButton, sr);
				sr->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->popScene();
			});
	}

	//IMPORTANT this need to be out of the isstarted!!!

	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
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
void ClockPuzzleScene::ResolveScene()
{
	Win();
}


