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


#include "AudioManager.h"

#include "SceneRoomTemplate.h"

TeaCupPuzzleScene::TeaCupPuzzleScene()
{
	_spoonIsInCup = false;
	_poisonIsChecked = false;
}

TeaCupPuzzleScene::~TeaCupPuzzleScene()
{

}

void TeaCupPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		room = sr;
		
		auto teaCupBackground = entityFactory->CreateImageEntity(entityManager, "TeaCupBackgroundWithoutSpoon", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		teaCupBackground->getMngr()->removeComponent<Area2D>(teaCupBackground);

		ecs::entity_t teaCupSpoon = entityFactory->CreateInteractableEntity( // Spoon entity
			entityManager, "TeaCupSpoon", EntityFactory::RECTAREA,
			Vector2D(100, 400), Vector2D(), 600, 400, 0,
			areaLayerManager, EntityFactory::DRAG, ecs::grp::DEFAULT);

		ecs::entity_t teaCup = entityFactory->CreateInteractableEntity( // Cup entity
			entityManager, "clockShape", EntityFactory::RECTAREA,
			Vector2D(400, 100), Vector2D(), 460, 280, 0,
			areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		teaCup->getMngr()->removeComponent<Image>(teaCup);


		entityManager->getComponent<ClickComponent>(teaCup) // The cup is clicked after introducing the spoon
			->connect(ClickComponent::JUST_CLICKED, [teaCupBackground, teaCup, this]()
				{
					if (_spoonIsInCup == false) return;
					_poisonIsChecked = true;

					// ... Change image revealing poinson or whatever  <-- TODO
					Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithPoison");
					teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
				});

		entityManager->addComponent<TriggerComponent>(teaCup) // Spoon enters the cup Area2D
			->connect(TriggerComponent::AREA_ENTERED, [teaCupBackground, this]()
				{
					_spoonIsInCup = true;
				});

		entityManager->getComponent<TriggerComponent>(teaCup) // Spoon leaves the cup Area2D
			->connect(TriggerComponent::AREA_LEFT, [this]()
				{
					_spoonIsInCup = false;
				});

		entityManager->getComponent<DragComponent>(teaCupSpoon) // The spoon is dropped in the cup
			->connect(DragComponent::DRAG_END, [teaCupBackground, teaCupSpoon, this]()
				{
					if (_spoonIsInCup == false) return;
					entityManager->setActive(teaCupSpoon, false);

					// ... Change image to cup with spoon  <-- TODO			
					Texture* tx = &sdlutils().images().at("TeaCupBackgroundWithSpoon");
					teaCupBackground->getMngr()->getComponent<Image>(teaCupBackground)->setTexture(tx);
				});

		//BackButton
		auto _backButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
		entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

		entityManager->addComponent<RectArea2D>(_backButton);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, []()
			{
				Game::Instance()->getSceneManager()->popScene();
			});
	}


}

void TeaCupPuzzleScene::refresh()
{

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
	return false;
}
