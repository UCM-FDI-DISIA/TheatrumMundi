#include "ParrotPuzzleScene.h"

#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/BehaviorStateComponent.h"
#include "../src/Components/RectArea2D.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/Image.h"
#include "Area2DLayerManager.h"
#include "Log.h"
#include "PauseManager.h"
#include "DialogueManager.h"
#include "../src/game/Game.h"
#include "../src/sdlutils/SDLUtils.h"
#include "AudioManager.h"
#include "Room3.h"

ParrotPuzzleScene::ParrotPuzzleScene()
{
	dialogueManager = new DialogueManager(5);
}

ParrotPuzzleScene::~ParrotPuzzleScene()
{

}

void ParrotPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		room = sr;
		dialogueManager->setScene(this);

		_setRoomAudio();

		_setGlobalFeatures();

		_setRoomBackground();

		_setInteractuables(sr);

		_setUI();

		_setDialog();
	}
	else if (sr->GetInventory()->hasItem("Linterna")) {
		if (Game::Instance()->getDataManager()->GetCharacterState(SOL) && Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("LORO2_2P");
		else {
			if (Game::Instance()->getDataManager()->GetCharacterState(SOL))startDialogue("LORO2_1PS");
			else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("LORO2_1PK");
		}
	}
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr,false); //the lantern is not going to dissapir
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

void ParrotPuzzleScene::_setRoomAudio()
{
	//Audio sfx 
	

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioManager().setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioManager().setVolume(rmSounds.puzzleButton, 0.3);

	rmSounds.doorSound = sdlutils().soundEffects().at("puerta");

	rmSounds.explosionSound = sdlutils().soundEffects().at("explosion");

	rmSounds.s_Sound = sdlutils().soundEffects().at("sSound");
	rmSounds.t_Sound = sdlutils().soundEffects().at("tSound");
	rmSounds.o_Sound = sdlutils().soundEffects().at("oSound");
	rmSounds.p_Sound = sdlutils().soundEffects().at("pSound");

	rmSounds.morse_Sound = sdlutils().soundEffects().at("MorseSound");
	rmSounds.shootSound = sdlutils().soundEffects().at("shootShound");

}

void ParrotPuzzleScene::_setRoomBackground()
{
	rmObjects.background = entityFactory->CreateImageEntity(entityManager, "Parrot", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
}

void ParrotPuzzleScene::_setInteractuables(SceneRoomTemplate* sr)
{
	// BULLETS
	int variant = Game::Instance()->getDataManager()->GetRoomVariant(2);
	Vector2D bulletsPosition(600,500);

	if (variant == 1) {
		rmObjects.bulletsEntity = entityFactory->CreateInteractableEntity(entityManager, "balasF" , EntityFactory::RECTAREA, // TODO Imagen balas
			bulletsPosition, Vector2D(0, 0), 150, 150, 0,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);
		entityManager->setActive(rmObjects.bulletsEntity, false);
	}
	else {
		rmObjects.bulletsEntity = entityFactory->CreateInteractableEntity(entityManager, "balasR", EntityFactory::RECTAREA, // TODO Imagen balas
			bulletsPosition, Vector2D(0, 0), 150, 150, 0,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);
		entityManager->setActive(rmObjects.bulletsEntity, false);
	}
	//entityManager->getComponent<ClickComponent>(rmObjects.bulletsEntity) // Collectable
	//	->connect(ClickComponent::JUST_CLICKED, [&, this]() {
	//		rmObjects.bulletsEntity->getMngr()->setActive(rmObjects.bulletsEntity, false);
	//		Vector2D position = sr->GetInventory()->setPosition();
	//		if (variant == 1)AddInvItem("balasF", sdlutils().Instance()->invDescriptions().at("BalasFalsas"), position, sr); // TODO Imagen balas
	//		else AddInvItem("balasR", sdlutils().Instance()->invDescriptions().at("BalasReales"), position, sr);
	//	});

	ClickComponent* clk = entityManager->getComponent<ClickComponent>(rmObjects.bulletsEntity);
	clk->connect(ClickComponent::JUST_CLICKED, [this,sr,variant]() {

		rmObjects.bulletsEntity->getMngr()->setActive(rmObjects.bulletsEntity, false);
		Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
		if (variant == 1) AddInvItem("balasF", sdlutils().Instance()->invDescriptions().at("BalasFalsas"), position, sr); // TODO Imagen balas
		else AddInvItem("balasR", sdlutils().Instance()->invDescriptions().at("BalasReales"), position, sr);
		entityManager->setActive(_backButton, true);
		});

	// PARROT
	Vector2D parrotPosition((sdlutils().width() - 700) / 2, (sdlutils().height() - 700) / 2);
	parrotUtils.parrotEnt = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, parrotPosition, Vector2D(0, 0), 700, 700, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

	parrotStateCom = entityManager->addComponent<BehaviorStateComponent>(parrotUtils.parrotEnt);

	parrotUtils.codeSequenceSounds.push_back(rmSounds.shootSound); // TODO: Gunshoot
	parrotUtils.codeSequenceSounds.push_back(rmSounds.s_Sound); // TODO: S
	parrotUtils.codeSequenceSounds.push_back(rmSounds.t_Sound); // TODO: T
	parrotUtils.codeSequenceSounds.push_back(rmSounds.o_Sound); // TODO: O
	parrotUtils.codeSequenceSounds.push_back(rmSounds.p_Sound); // TODO: P
	parrotUtils.codeSequenceSounds.push_back(rmSounds.explosionSound); //TODO: Explosion

	auto shootingBehavior = [/*parrotStateCom,*/ this] // SHOOTING_SOUND
		() {
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000 && Game::Instance()->getDataManager()->GetRoom3Phase() == 1) { // Every second

				audioManager().setVolume(rmSounds.shootSound,1.0);
				audioManager().playSound(parrotUtils.codeSequenceSounds[0]);
				parrotUtils.lastSoundTime = sdlutils().currTime();
			}	

			parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		};

	auto stopBehavior = [/*parrotStateCom, */this]
		() {
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000 && Game::Instance()->getDataManager()->GetRoom3Phase() == 2) { // Every second

				//AudioManager& audioMngr = AudioManager::Instance();

				if (parrotUtils.codeSeqIteration == parrotUtils.codeSequenceSounds.size() - 1) {
					parrotUtils.codeSeqIteration++;
					parrotUtils.codeSeqIteration++;
					parrotUtils.codeSeqIteration = parrotUtils.codeSeqIteration % parrotUtils.codeSequenceSounds.size();
				} //Not repeat the explosion

				if (parrotUtils.codeSeqIteration == 0) {
					++parrotUtils.codeSeqIteration;
				} //Not repeat the shoot

				audioManager().playSound(parrotUtils.codeSequenceSounds[parrotUtils.codeSeqIteration]);

				++parrotUtils.codeSeqIteration;

				parrotUtils.codeSeqIteration = parrotUtils.codeSeqIteration % parrotUtils.codeSequenceSounds.size();

				parrotUtils.lastSoundTime = sdlutils().currTime();
			}
			parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		};

	parrotStateCom->defBehavior(Room3Scene::Phase::LIGHTS_OFF, shootingBehavior); // Both phases have the same behavior in the parrot
	parrotStateCom->defBehavior(Room3Scene::Phase::LIGHTS_ON, shootingBehavior);

	parrotStateCom->defBehavior(Room3Scene::Phase::LIGHTS_RED, stopBehavior);

	parrotStateCom->defBehavior(ParrotState::DEATH, [](){}); // Death the parrot doen't do anithing

	parrotStateCom->setState(Room3Scene::Phase::LIGHTS_OFF); // The other will be setted after finishin the puzzle

	parrotUtils.parrotEnt->getMngr()->getComponent<TriggerComponent>(parrotUtils.parrotEnt)->connect(TriggerComponent::AREA_ENTERED, [this]() {
		SetplacedHand(true);
		});

	parrotUtils.parrotEnt->getMngr()->getComponent<TriggerComponent>(parrotUtils.parrotEnt)->connect(TriggerComponent::AREA_LEFT, [this]() {
		SetplacedHand(false);
		});
}

void ParrotPuzzleScene::_setDialog()
{
	// Dialog
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "Sala3");

	assert(inventoryButton != nullptr); // UI must be Initialized First

	Area2D* inventoryButtonArea = entityManager->getComponent<Area2D>(inventoryButton);

	auto dialogEnts = entityManager->getEntities(ecs::grp::DIALOGUE);

	for (ecs::entity_t dialogEnt : dialogEnts)
	{
		Area2D* dialogArea = entityManager->getComponent<Area2D>(dialogEnt);
		if (dialogArea != nullptr)
			areaLayerManager->sendAfter(inventoryButtonArea->getLayerPos(), dialogArea->getLayerPos());
	}
	dialogueManager->setScene(this);
	std::cout << Game::Instance()->getDataManager()->GetCharacterState(SOL) << " slo " << Game::Instance()->getDataManager()->GetCharacterState(KEISARA) << std::endl;
	if (Game::Instance()->getDataManager()->GetCharacterState(SOL) && Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("LORO1_2P");
	else {
		if (Game::Instance()->getDataManager()->GetCharacterState(SOL))startDialogue("LORO1_1PS");
		else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("LORO1_1PK");
	}
}

void ParrotPuzzleScene::_setUI()
{

	//createInventoryUI();
	//
	//// Corpse zoom Quit Button
	//rmObjects.quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
	//
	//entityManager->getComponent<ClickComponent>(rmObjects.quitButton)
	//	->connect(ClickComponent::JUST_CLICKED, [this]()
	//		{
	//			AudioManager::Instance().playSound(rmSounds.uiButton);
	//			Game::Instance()->getSceneManager()->popScene();
	//		});
	//
	//entityManager->setActive(rmObjects.quitButton, false);
	//
	//// Pause Logic
	//pauseManager->setScene(this);
	//pauseManager->Init(entityFactory, entityManager, areaLayerManager);
	//
	//areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getbackgroundNotInteractable())->getLayerPos());
	//areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getreanudePauseButton())->getLayerPos());
	//areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getexitPauseButton())->getLayerPos());
	//
	//logbtn = rmObjects.logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);


	//AudioManager& a = AudioManager::Instance();
	std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
	audioManager().setVolume(buttonSound, 0.2);

	//BackButton
		//ENTIDADCONENTITYFACTORY
	 _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

	//INVENTORY
		//Invntory Background
	createInventoryUI();
	//Click component Open log button
	ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
	clkOpen->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
		{
			//AudioManager::Instance().playSound(buttonSound);

			inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
			HideInventoryItems();
			room->GetInventory()->setFirstItem(0);
			auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
			_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
			_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);

			audioManager().stopSound(rmSounds.shootSound);
			audioManager().stopSound(rmSounds.explosionSound);
			audioManager().stopSound(rmSounds.morse_Sound);
			audioManager().stopSound(rmSounds.s_Sound);
			audioManager().stopSound(rmSounds.t_Sound);
			audioManager().stopSound(rmSounds.o_Sound);
			audioManager().stopSound(rmSounds.p_Sound);

			parrotUtils.lastSoundTime = 0;

			Game::Instance()->getSceneManager()->popScene();
		});
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
	logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
	dialogueManager->setScene(this);

}

bool ParrotPuzzleScene::isItemHand(const std::string& itemId)
{

	if (itemId == "Linterna" && Game::Instance()->getDataManager()->GetRoom3Phase() == 2) {
		audioManager().playSound(rmSounds.explosionSound);						// TODO: Exploded_Parrot image
		entityManager->getComponent<Image>(parrotUtils.parrotEnt)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<ClickComponent>(rmObjects.bulletsEntity)->setLayerOpposition(false); // We can collect the bullets even with the parrot on top
		//parrotStateCom->setState(ParrotState::DEATH);
		parrotUtils.parrotEnt->getMngr()->setActive(parrotUtils.parrotEnt,false);
		entityManager->setActive(_backButton, false);
		Win();
		return true;
	}
	return false;
}

void ParrotPuzzleScene::Win()
{
	rmObjects.background->getMngr()->getComponent<Image>(rmObjects.background)->setTexture(&sdlutils().images().at("deadParrot"));
	room->resolvedPuzzle(3);

	entityManager->setActive(rmObjects.bulletsEntity, true);
}

void ParrotPuzzleScene::ResolveScene()
{
	Win();
}



