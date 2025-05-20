#include "CorpseLucyScene.h"
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

CorpseLucyScene::CorpseLucyScene()
{
	dialogueManager = new DialogueManager(1);
}

CorpseLucyScene::~CorpseLucyScene()
{
}

void CorpseLucyScene::init(SceneRoomTemplate* sr)
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
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr, false); //the lantern is not going to dissapir

	startDialogue("Cadaver");
}

void CorpseLucyScene::unload()
{
}

bool CorpseLucyScene::Check()
{
	return false;
}

void CorpseLucyScene::refresh()
{
	if (isAnimating && frameTimerCorpse.currRealTime() > 400) // 0.4 sec
	{
		entityManager->getComponent<Image>(rmObjects.background)->setTexture(&sdlutils().images().at("B1"));
	}
}

void CorpseLucyScene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void CorpseLucyScene::_setRoomAudio()
{
	//Audio sfx 
	/*AudioManager& audioMngr = AudioManager::Instance();*/

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioManager().setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioManager().setVolume(rmSounds.puzzleButton, 0.3);

}

void CorpseLucyScene::_setRoomBackground()
{
	rmObjects.background = entityFactory->CreateImageEntity(entityManager, "zoomCadaverAzul", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
}

void CorpseLucyScene::_setInteractuables(SceneTemplate* sr)
{
	rmObjects.interactuableArea = entityFactory->CreateInteractableEntity(entityManager, "Parrot", EntityFactory::RECTAREA, Vector2D((sdlutils().width() - 700) / 2, (sdlutils().height() - 700) / 2), Vector2D(0, 0), 700, 700, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

	rmObjects.interactuableArea->getMngr()->getComponent<TriggerComponent>(rmObjects.interactuableArea)->connect(TriggerComponent::AREA_ENTERED, [this]() {
		SetplacedHand(true);
		});

	rmObjects.interactuableArea->getMngr()->getComponent<TriggerComponent>(rmObjects.interactuableArea)->connect(TriggerComponent::AREA_LEFT, [this]() {
		SetplacedHand(false);
		});
}

void CorpseLucyScene::_setDialog()
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
}

void CorpseLucyScene::_setUI()
{
	//AudioManager& a = AudioManager::Instance();
	std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
	audioManager().setVolume(buttonSound, 0.2);

	//BackButton
	//ENTIDADCONENTITYFACTORY
	auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

	//INVENTORY
	//Invntory Background
	createInventoryUI();
	//Click component Open log button
	ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
	clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton, buttonSound]()
		{
			audioManager().playSound(buttonSound);

			inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
			HideInventoryItems();
			room->GetInventory()->setFirstItem(0);
			auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
			_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
			_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);

			Game::Instance()->getSceneManager()->popScene();
		});
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
	logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
	dialogueManager->setScene(this);
}

bool CorpseLucyScene::isItemHand(const std::string& itemId)
{

	if (itemId == "Linterna" && Game::Instance()->getDataManager()->GetRoom3Phase() == 2) {
		entityManager->getComponent<Image>(rmObjects.background)->setTexture(&sdlutils().images().at("Parrot"));
		frameTimerCorpse.resetTime();
		isAnimating = true;
		return true;
	}
	return false;
}
