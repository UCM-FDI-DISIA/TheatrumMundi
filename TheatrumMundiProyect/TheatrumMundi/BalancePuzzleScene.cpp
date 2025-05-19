#include "BalancePuzzleScene.h"

#include "EntityFactory.h"
#include "../src/utils/Vector2D.h"
#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/DragComponent.h"
#include "../src/Components/Area2D.h"
#include "../src/Components/Image.h"
#include "../src/sdlutils/VirtualTimer.h"
#include "../src/sdlutils/SDLUtils.h"

#include "../src/components/Transform.h"

#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/Components/WriteTextComponent.h"

#include "AudioManager.h"

#include "SceneRoomTemplate.h"

#include <string>

#include "../../TheatrumMundi/DescriptionInfo.h"

#include "DialogueManager.h"

#include "Log.h"

using namespace std;

BalancePuzzleScene::BalancePuzzleScene()
{
	_featherIsInBalance = false;
	_hasFeather = false;

	dialogueManager = new DialogueManager(1);
}

BalancePuzzleScene::~BalancePuzzleScene()
{

}

void BalancePuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {

		isStarted = true;
		room = sr;

		AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		dialogueManager->setScene(this);

		balanceBackground = entityFactory->CreateImageEntity(entityManager, "balance", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		balanceBackground->getMngr()->removeComponent<Area2D>(balanceBackground);
		

		auto balanceArea = entityFactory->CreateInteractableEntity( // balance entity
			entityManager, "balance", EntityFactory::RECTAREA,
			Vector2D(800, 450), Vector2D(), 200, 100, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		balanceArea->getMngr()->removeComponent<Image>(balanceArea);



		
		feather = entityFactory->CreateInteractableEntity( // feather entity
			entityManager, "pluma", EntityFactory::RECTAREA,
			Vector2D(400, 300), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		if (!_hasFeather) feather->getMngr()->setActive(feather, false);

		balanceArea->getMngr()->getComponent<TriggerComponent>(balanceArea)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			std::cout << "pasa por el triger de la balanza" << std::endl;
			});


		//Assigns the trigger bolean to false
		balanceArea->getMngr()->getComponent<TriggerComponent>(balanceArea)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});



		//BackButton
		//ENTIDADCONENTITYFACTORY
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, _backButton, buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);

				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				sr->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->popScene();

			});
		
		//INVENTORY
		//Invntory Background
		createInventoryUI();
		//Click component Open log button
		clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
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
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
	//	startDialogue("PuzzleTaza1");
	}
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
}

void BalancePuzzleScene::init()
{
}


void BalancePuzzleScene::unload()
{

}

bool BalancePuzzleScene::Check()
{

	return _featherIsInBalance;
}

bool BalancePuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "pluma") {
		_hasFeather = true;
		_featherIsInBalance = true;
		Image* img = balanceBackground->getMngr()->getComponent<Image>(balanceBackground);
		img->setTexture(&sdlutils().images().at("FeatherBackground"));
		//feather->getMngr()->setActive(feather, true);
		Win();
		return true;
	}
	return false;
}
void BalancePuzzleScene::Win()
{
	
	
	
	room->resolvedPuzzle(0);
}

void BalancePuzzleScene::ResolveScene()
{
	Win();
}
