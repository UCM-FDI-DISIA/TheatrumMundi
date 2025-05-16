#include "ParrotPuzzleScene.h"

#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/BehaviorStateComponent.h"
#include "../src/Components/RectArea2D.h"
#include "Area2DLayerManager.h"
#include "Log.h"
#include "PauseManager.h"
#include "DialogueManager.h"
#include "../src/game/Game.h"
#include "../src/sdlutils/SDLUtils.h"
#include "AudioManager.h"

ParrotPuzzleScene::ParrotPuzzleScene()
{

}

ParrotPuzzleScene::~ParrotPuzzleScene()
{

}

void ParrotPuzzleScene::init(SceneRoomTemplate* sr)
{
	_setRoomAudio();

	_setGlobalFeatures();

	_setRoomBackground();

	_setInteractuables();

	_setUI();

	_setDialog();
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
	AudioManager& audioMngr = AudioManager::Instance();

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioMngr.setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioMngr.setVolume(rmSounds.puzzleButton, 0.3);

	rmSounds.doorSound = sdlutils().soundEffects().at("puerta");

	rmSounds.explosionSound = sdlutils().soundEffects().at("explosion");

	rmSounds.s_Sound = sdlutils().soundEffects().at("sSound");
	rmSounds.t_Sound = sdlutils().soundEffects().at("tSound");
	rmSounds.o_Sound = sdlutils().soundEffects().at("oSound");
	rmSounds.p_Sound = sdlutils().soundEffects().at("pSound");

	rmSounds.morse_Sound = sdlutils().soundEffects().at("MorseSound");
}

void ParrotPuzzleScene::_setRoomBackground()
{
	entityFactory->CreateImageEntity(entityManager, "Parrot", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
}

void ParrotPuzzleScene::_setInteractuables()
{
	parrotUtils.parrotEnt = entityFactory->CreateInteractableEntity(entityManager, "ParrotOscuro", EntityFactory::RECTAREA, Vector2D(1000, 0), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<TriggerComponent>(parrotUtils.parrotEnt)->connect(TriggerComponent::AREA_ENTERED, [this]() {
		// when the torch enters
		});

	BehaviorStateComponent* parrotStateCom = entityManager->addComponent<BehaviorStateComponent>(parrotUtils.parrotEnt);

	parrotUtils.codeSequenceSounds.push_back(rmSounds.explosionSound); // TODO: Gunshoot
	parrotUtils.codeSequenceSounds.push_back(rmSounds.s_Sound); // TODO: S
	parrotUtils.codeSequenceSounds.push_back(rmSounds.t_Sound); // TODO: T
	parrotUtils.codeSequenceSounds.push_back(rmSounds.o_Sound); // TODO: O
	parrotUtils.codeSequenceSounds.push_back(rmSounds.p_Sound); // TODO: P

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
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "Sala3");

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
				AudioManager::Instance().playSound(rmSounds.uiButton);
				Game::Instance()->getSceneManager()->popScene();
			});

	entityManager->setActive(rmObjects.quitButton, false);


	createInventoryUI();

	// Pause Logic
	pauseManager->setScene(this);
	pauseManager->Init(entityFactory, entityManager, areaLayerManager);

	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getbackgroundNotInteractable())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getreanudePauseButton())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getexitPauseButton())->getLayerPos());

	logbtn = rmObjects.logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
}
