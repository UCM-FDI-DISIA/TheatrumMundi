#include "TeaCupPuzzleScene.h"

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

TeaCupPuzzleScene::TeaCupPuzzleScene()
{
	_spoonIsInCup = false;
	_poisonIsChecked = false;

	dialogueManager = new DialogueManager(1);
}

TeaCupPuzzleScene::~TeaCupPuzzleScene()
{

}

void TeaCupPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		
		isStarted = true;
		room = sr;
		
		//AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		audioManager().setVolume(buttonSound, 0.2);

		dialogueManager->setScene(this);

		teaCupBackground = entityFactory->CreateImageEntity(entityManager, "TeaCupBackgroundWithoutSpoon", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		teaCupBackground->getMngr()->removeComponent<Area2D>(teaCupBackground); //Disable the area, this allows to change the background without errors (NOT REMOVE THIS LINE)

		ecs::entity_t teaCup = entityFactory->CreateInteractableEntity( // Cup entity
			entityManager, "EmptyImage", EntityFactory::RECTAREA,
			Vector2D(400, 100), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		teaCup->getMngr()->getComponent<TriggerComponent>(teaCup)->setTargetGroup(ecs::grp::INVENTORY);
		teaCup->getMngr()->getComponent<TriggerComponent>(teaCup)->connect(TriggerComponent::AREA_ENTERED, [this]() {
			SetplacedHand(true);
			std::cout << "pasa por el triger de la taza" << std::endl;
			});
		//Assigns the trigger bolean to false
		teaCup->getMngr()->getComponent<TriggerComponent>(teaCup)->connect(TriggerComponent::AREA_LEFT, [this]() {
			SetplacedHand(false);
			});

		entityManager->getComponent<ClickComponent>(teaCup) // The cup is clicked after introducing the spoon
			->connect(ClickComponent::JUST_CLICKED, [teaCup, this]()
				{
					if (_spoonIsInCup == false) return;
					_poisonIsChecked = true;
					teaCup->getMngr()->setActive(teaCup, false); //Disable the area to lay the spoon
					// ... Change image revealing poinson or whatever  <-- TODO
					Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithPoison");
					teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
					startDialogue("PuzzleTaza2");
				});


		//BackButton
		//ENTIDADCONENTITYFACTORY
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);


		//INVENTORY
		createInventoryUI();
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, _backButton, buttonSound]()
		{
				audioManager().playSound(buttonSound);

			
			HideInventoryItems();
			sr->GetInventory()->setFirstItem(0);
			auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
			_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
			_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
			_backButtonImage->setPosOffset(0, 0);
			Game::Instance()->getSceneManager()->popScene();
		});
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);


		
	}
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);

	startDialogue("PuzzleTaza1");
}


void TeaCupPuzzleScene::unload()
{

}

bool TeaCupPuzzleScene::Check()
{
	return _poisonIsChecked;
}

bool TeaCupPuzzleScene::isItemHand(const std::string& itemId)
{
	if (itemId == "TeaCupSpoon") {
		std::shared_ptr<Sound> spoonSound = sdlutils().soundEffects().at("taza");
		audioManager().playSound(spoonSound);
		_spoonIsInCup = true; 
		Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithSpoon");
		teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
		//spoon->getMngr()->setActive(spoon, true); 
		return true;
	}
	return false;
}
