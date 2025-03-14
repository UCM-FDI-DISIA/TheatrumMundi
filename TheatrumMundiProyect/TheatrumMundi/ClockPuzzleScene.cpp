#include "ClockPuzzleScene.h"

#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/LogComponent.h"
#include <cassert>
#include "AudioManager.h"

#include "SceneRoomTemplate.h"

ClockPuzzleScene::ClockPuzzleScene() : ScenePuzzleTemplate()
{
	_actualHour = 90;
	_actualMinute = 0;
	hasLongCloackHand = false;
	hasShortCloackHand = false;
	placeHand = false;
}

ClockPuzzleScene::~ClockPuzzleScene()
{
}

void ClockPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {


		isStarted = true;
		//Register scene in dialogue manager
		Game::Instance()->getDialogueManager()->setScene(this);


		//All Screen: Object to detect click on screen. Used to read displayed dialogue.
		auto _screenDetect = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->addComponent<Image>(_screenDetect, &sdlutils().images().at("fondoPruebaLog"), 100); //background log
		entityManager->addComponent<RectArea2D>(_screenDetect, areaLayerManager);
		entityManager->addComponent<TriggerComponent>(_screenDetect);
		entityManager->setActive(_screenDetect, false);



		//Create dialogue text entity. Object that renders dialogue Text on Screen
		auto _textbackground = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_textbackground, Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0);
		entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
		entityManager->addComponent<RectArea2D>(_textbackground, areaLayerManager);

		entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
			{
				if (!logActive) {
					//read dialogue only if it has to
					if (Game::Instance()->getDialogueManager()->getDisplayOnProcess())
					{
						Game::Instance()->getDialogueManager()->ReadDialogue(Puzzle3);
					}
					else
					{
						_textbackground->getMngr()->setActive(_textbackground, false);
					}
				}
			});
		entityManager->addComponent<TriggerComponent>(_textbackground);
		entityManager->setActive(_textbackground, false);

		//CharacterImage
		auto characterimg = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(characterimg, Vector2D(0, 200), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
		auto imCh = entityManager->addComponent<Image>(characterimg, &sdlutils().images().at("Dialog"));

		Game::Instance()->getDialogueManager()->setCharacterImg(imCh);
		entityManager->setActive(characterimg, false);

		auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		entityManager->setActive(_textTest, false);


		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);

		startDialogue(Puzzle3);

		room = sr;
		AudioManager& a = AudioManager::Instance();
		Sound clockMinSound = sdlutils().soundEffects().at("aguja_minutero");
		Sound clockHorSound = sdlutils().soundEffects().at("aguja_horario");
		a.setVolume(clockMinSound, 0.2);
		a.setVolume(clockHorSound, 0.2);

		//create the clock
		cloack = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(600, 300), Vector2D(0, 0), 200, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		entityManager->addComponent<TriggerComponent>(cloack);
		//Assigns the trigger bolean to true
		cloack->getMngr()->getComponent<TriggerComponent>(cloack)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			placeHand = true;
			});
		//Assigns the trigger bolean to false
		cloack->getMngr()->getComponent<TriggerComponent>(cloack)->connect(TriggerComponent::AREA_LEFT, [this]() {
			placeHand = false;
			});

		//create the clock hands : minute
		longCloackHand = entityFactory->CreateInteractableEntity(entityManager, "clockMinArrow", EntityFactory::RECTAREA, Vector2D(680, 360), Vector2D(0, 0), 20, 70, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockMinTransform = longCloackHand->getMngr()->getComponent<Transform>(longCloackHand);
		if (!hasLongCloackHand) longCloackHand->getMngr()->setActive(longCloackHand, false);

		//create the clock hands : hour
		shortCloackHand = entityFactory->CreateInteractableEntity(entityManager, "clockHorArrow", EntityFactory::RECTAREA, Vector2D(695, 360), Vector2D(0, 0), 20, 60, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BACKGROUND);
		auto _clockHorTransform = shortCloackHand->getMngr()->getComponent<Transform>(shortCloackHand);
		if (!hasShortCloackHand) shortCloackHand->getMngr()->setActive(shortCloackHand, false);

		//create the buttons: min
		auto _buttonMin = entityManager->addEntity();
		auto _buttonMinTransform = entityManager->addComponent<Transform>(_buttonMin, Vector2D(200, 600), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonMin, &sdlutils().images().at("clockMinButton"));

		entityManager->addComponent<RectArea2D>(_buttonMin);

		_actualMinute;

		ClickComponent* clockMinClick = entityManager->addComponent<ClickComponent>(_buttonMin);
		clockMinClick->connect(ClickComponent::JUST_CLICKED, [_clockMinTransform, clockMinSound, this]()
			{
#ifdef DEBUG
				std::cout << "CLICKED MINUTERO\n";
#endif // DEBUG
				if (hasLongCloackHand) {
					AudioManager::Instance().playSound(clockMinSound);
					_clockMinTransform->setRot(_clockMinTransform->getRot() + 15);
					_actualMinute += 15;
					if (_actualMinute == 360) _actualMinute = 0;
				}
			});


		//create the buttons: hor
		auto _buttonHor = entityManager->addEntity();
		auto _buttonHorTransform = entityManager->addComponent<Transform>(_buttonHor, Vector2D(200, 550), Vector2D(0, 0), 30, 30, 0);

		entityManager->addComponent<Image>(_buttonHor, &sdlutils().images().at("clockHorButton"));

		entityManager->addComponent<RectArea2D>(_buttonHor);


		ClickComponent* clockHorClick = entityManager->addComponent<ClickComponent>(_buttonHor);
		clockHorClick->connect(ClickComponent::JUST_CLICKED, [_clockHorTransform, clockHorSound, this]()
			{
#ifdef DEBUG
				std::cout << "CLICKED HORARIO\n";
#endif // DEBUG
				if (hasShortCloackHand) {
					AudioManager::Instance().playSound(clockHorSound);
					_clockHorTransform->setRot(_clockHorTransform->getRot() + 30);
					_actualHour += 30;
					if (_actualHour == 360) _actualHour = 0;
				}
			});


		//create the buttons: check
		auto _buttonCheck = entityManager->addEntity();
		auto _buttonCheckTransform = entityManager->addComponent<Transform>(_buttonCheck, Vector2D(100, 550), Vector2D(0, 0), 70, 70, 0);

		entityManager->addComponent<Image>(_buttonCheck, &sdlutils().images().at("clockCheckButton"));

		entityManager->addComponent<RectArea2D>(_buttonCheck);


		ClickComponent* clockCheckClick = entityManager->addComponent<ClickComponent>(_buttonCheck);
		clockCheckClick->connect(ClickComponent::JUST_CLICKED, [_buttonCheckTransform, sr, this]()
			{
				if (Check()) {

					//Assigns to the room inventory the hint

					Vector2D position = sr->GetInventory()->setPosition();
					sr->GetInventory()->addItem(new Hint("AAA", "Me lo puedo beber??", &sdlutils().images().at("AAA")));
					sr->GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(sr->GetEntityManager(), "AAA", EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
					sr->GetInventory()->hints.back()->getMngr()->setActive(sr->GetInventory()->hints.back(), false);

					//Assign to this inventory the hint;
					invID.push_back("AAA");
					invObjects.push_back(entityFactory->CreateInteractableEntity(entityManager, "AAA", EntityFactory::RECTAREA, position, Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT));
					invObjects.back()->getMngr()->setActive(invObjects.back(), false);

					//Assign the lamda functions to the inventory item
					invObjects.back()->getMngr()->getComponent<ClickComponent>(invObjects.back())->connect(ClickComponent::JUST_CLICKED, [this, sr]() {
						setOriginalPos(invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos());
						});

					//if you drop the item, compares if it was drop in or out tge cloack
					invObjects.back()->getMngr()->getComponent<ClickComponent>(invObjects.back())->connect(ClickComponent::JUST_RELEASED, [this, sr]() {
						//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
						if (!placeHand) invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos().set(getOriginalPos());
						//in other case remove the item from this inventory and the inventory of Room1
						else {
							//Add the hand to the cloack
							if (isCloackHand("AAA")) {

								//remove the object from the inventory
								sr->GetInventory()->removeItem("AAA", invObjects);
							}
							else invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos().set(getOriginalPos());
						}
						});
#ifdef DEBUG
					std::cout << "wii";
#endif // DEBUG
					Win();

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
#ifdef DEBUG
				std::cout << "WAAAAAAAAAA\n";
#endif // DEBUG

				_clockHorTransform->setRot(90);
				_actualHour = 90;
				_clockMinTransform->setRot(0);
				_actualMinute = 0;
			});


		//BackButton
		auto _backButton = entityManager->addEntity(ecs::grp::UI);
		entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));
		entityManager->addComponent<RectArea2D>(_backButton);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [sr]()
			{
				Game::Instance()->getSceneManager()->popScene();
			});

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(0, 0), Vector2D(0, 0), 1500, 1500, 0, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);

		//InventoryButton
		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground]() //Lamda function
			{
				//AudioManager::Instance().playSound(buttonSound);
				sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

				// If the inventory is active, activate the items
				if (sr->GetInventory()->getActive()) {
					//	entityManager->setActive(InventoryBackground, true);
					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
					//	entityManager->setActive(InventoryBackground, false);
					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], false);
					}
				}
			});

	}
	//IMPORTANT this need to be out of the isstarted!!!
	//Add inventory items to the entitymanager of the scene

	int index = 0;
	for (auto a : sr->GetInventory()->getItems()) {
		//if the array of names hasn't have the name of this entity then it means that is new and has to be created again
		if (!ItemAlreadyCreated(a->getID())) {

			//Add the item name to the array names
			invID.push_back(a->getID());
			//Add the entitie to the array
			invObjects.push_back(entityFactory->CreateInteractableEntity(entityManager, a->getID(), EntityFactory::RECTAREA, sr->GetInventory()->GetPosition(index), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT));


			//Assign lamda functions
			
			//if you click in one item, assign the original position to the position of the object clicked
			invObjects.back()->getMngr()->getComponent<ClickComponent>(invObjects.back())->connect(ClickComponent::JUST_CLICKED, [this, sr]() {
				setOriginalPos(invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos());
				});

			//if you drop the item, compares if it was drop in or out tge cloack
			invObjects.back()->getMngr()->getComponent<ClickComponent>(invObjects.back())->connect(ClickComponent::JUST_RELEASED, [this, sr, a]() {
				//if the item is invalid or the player drop it at an invalid position return the object to the origianl position
				if (!placeHand) invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos().set(getOriginalPos());
				//in other case remove the item from this inventory and the inventory of Room1
				else {
					//Add the hand to the cloack
					if (isCloackHand(a->getID())) { 
						
						//remove the object from the inventory
						sr->GetInventory()->removeItem(a->getID(),invObjects);
					}
					else invObjects.back()->getMngr()->getComponent<Transform>(invObjects.back())->getPos().set(getOriginalPos());
				}
			});

			//Set the active item to false
			invObjects[index]->getMngr()->setActive(invObjects[index], false);
			++index;
		}
	}
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
bool ClockPuzzleScene::isCloackHand(const std::string& itemId)
{
	if (itemId == "boa2") {
		hasLongCloackHand = true;
		longCloackHand->getMngr()->setActive(longCloackHand, true);
		return true;
	}
	else if (itemId == "TeaCupSpoon") {
		hasShortCloackHand = true;
		shortCloackHand->getMngr()->setActive(shortCloackHand, true);
		return true;
	}
	return false;
}

bool ClockPuzzleScene::Check()
{
	if (_actualHour == 180 && _actualMinute == 180) return true;
	else 
	return false;
}

void ClockPuzzleScene::Win()
{
	room->resolvedPuzzle(7);
}

