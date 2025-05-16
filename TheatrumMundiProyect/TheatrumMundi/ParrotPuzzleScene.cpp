#include "ParrotPuzzleScene.h"

#include "../src/Components/TriggerComponent.h"
#include "../src/Components/BehaviorStateComponent.h"
#include "../src/game/Game.h"
#include "../src/sdlutils/SDLUtils.h"

ParrotPuzzleScene::ParrotPuzzleScene()
{

}

ParrotPuzzleScene::~ParrotPuzzleScene()
{

}

void ParrotPuzzleScene::init(SceneRoomTemplate* sr)
{

}

void ParrotPuzzleScene::unload()
{

}

bool ParrotPuzzleScene::Check()
{
	return false;
}

// Init Setting Methods

void ParrotPuzzleScene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void ParrotPuzzleScene::_setRoomBackground()
{
	teaCupBackground = entityFactory->CreateImageEntity(entityManager, "Parrot", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
}

void ParrotPuzzleScene::_setInteractuables()
{
	parrotUtils.parrotEnt = entityFactory->CreateInteractableEntity(entityManager, "ParrotOscuro", EntityFactory::RECTAREA, Vector2D(1000, 0), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<TriggerComponent>(parrotUtils.parrotEnt)->connect(TriggerComponent::AREA_ENTERED, [this]() {
		// when the torch enters
		});

	BehaviorStateComponent* parrotStateCom = entityManager->addComponent<BehaviorStateComponent>(parrotUtils.parrotEnt);

	parrotUtils.codeSequenceSounds.push_back(rmSounds.explosionSound); // TODO: Gunshoot
	parrotUtils.codeSequenceSounds.push_back(rmSounds.doorSound); // TODO: S
	parrotUtils.codeSequenceSounds.push_back(rmSounds.uiButton); // TODO: T
	// More sounds...

	parrotStateCom->defBehavior(ParrotState::SHOOTING_SOUND,
		[&]() {
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000) { // Every second
				AudioManager::Instance().playSound(parrotUtils.codeSequenceSounds[0]);
				parrotUtils.lastSoundTime = sdlutils().currTime();
			}

			parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		});

	parrotStateCom->defBehavior(ParrotState::RED_LIGHTS,
		[&]() {
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000) { // Every second

				AudioManager::Instance().playSound(parrotUtils.codeSequenceSounds[parrotUtils.codeSeqIteration]);

				++parrotUtils.codeSeqIteration;
				parrotUtils.codeSeqIteration = parrotUtils.codeSeqIteration % parrotUtils.codeSequenceSounds.size();

				parrotUtils.lastSoundTime = sdlutils().currTime();
			}

			parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		});

	parrotStateCom->setState(ParrotState::SHOOTING_SOUND); // The other will be setted after finishin the puzzle
}

void ParrotPuzzleScene::_setDialog()
{
	// Dialog
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, _eventToRead);

	assert(rmObjects.inventoryButton != nullptr); // UI must be Initialized First

	Area2D* inventoryButtonArea = entityManager->getComponent<Area2D>(rmObjects.inventoryButton);

	auto dialogEnts = entityManager->getEntities(ecs::grp::DIALOGUE);

	for (ecs::entity_t dialogEnt : dialogEnts)
	{
		Area2D* dialogArea = entityManager->getComponent<Area2D>(dialogEnt);
		if (dialogArea != nullptr)
			areaLayerManager->sendAfter(inventoryButtonArea->getLayerPos(), dialogArea->getLayerPos());
	}
}

void ParrotPuzzleScene::_setUI()
{
	// Corpse zoom Quit Button
	rmObjects.quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);

	entityManager->getComponent<ClickComponent>(rmObjects.quitButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				room->GetInventory()->setFirstItem(0);
				AudioManager::Instance().playSound(rmSounds.uiButton);
				Game::Instance()->getSceneManager()->popScene();
			});

	entityManager->setActive(rmObjects.quitButton, false);


	//inventory descriptions
	//description text entity
	invObjects.textDescriptionEnt = entityManager->addEntity(ecs::grp::UI);
	auto _testTextTranform = entityManager->addComponent<Transform>(invObjects.textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
	entityManager->setActive(invObjects.textDescriptionEnt, false);
	SDL_Color colorDialog = { 255, 255, 255, 255 };
	entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(invObjects.textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, GetInventory()->getTextDescription());


	//Inventory

	invObjects.InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);

	rmObjects.inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.InventoryBackground, false);

	invObjects.InvArea = entityManager->addComponent<RectArea2D>(invObjects.InventoryBackground, areaLayerManager);

	invObjects.inventoryUpButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryUpButton, false);

	invObjects.inventoryDownButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryDownButton, false);

	entityManager->getComponent<ClickComponent>(rmObjects.inventoryButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

				if (GetInventory()->getActive()) // If the inventory is active, activate the items
				{
					entityManager->setActive(invObjects.InventoryBackground, true);
					entityManager->setActive(rmObjects.logbtn, false);
					//change the position of the log button
					areaLayerManager->sendFront(invObjects.InvArea->getLayerPos());
					entityManager->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(925);

					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryUpButton)->getLayerPos());
					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryDownButton)->getLayerPos());

					entityManager->setActive(invObjects.inventoryDownButton, true);
					entityManager->setActive(invObjects.inventoryUpButton, true);

					for (int i = inv->getFirstItem(); i < inv->getItemNumber() + inv->getFirstItem(); ++i) {
						inv->hints[i]->getMngr()->setActive(inv->hints[i], true);  // Activate the items
						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inv->hints[i])->getLayerPos());
					}
				}
				else
				{
					entityManager->setActive(invObjects.InventoryBackground, false);
					entityManager->setActive(invObjects.inventoryDownButton, false);
					entityManager->setActive(invObjects.inventoryUpButton, false);
					entityManager->setActive(rmObjects.logbtn, true);
					rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
					for (int i = inv->getFirstItem(); i < inv->getItemNumber() + inv->getFirstItem(); ++i) inv->hints[i]->getMngr()->setActive(inv->hints[i], false);  // Desactivate the items 

				}
			});

	entityManager->getComponent<ClickComponent>(invObjects.inventoryUpButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				scrollInventory(-1);
			});
	entityManager->getComponent<ClickComponent>(invObjects.inventoryDownButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				scrollInventory(1);
			});

	entityManager->setActive(rmObjects.quitButton, false);

	// Pause Logic
	pauseManager->setScene(this);
	pauseManager->Init(entityFactory, entityManager, areaLayerManager);

	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getbackgroundNotInteractable())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getreanudePauseButton())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getexitPauseButton())->getLayerPos());

	logbtn = rmObjects.logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
}
