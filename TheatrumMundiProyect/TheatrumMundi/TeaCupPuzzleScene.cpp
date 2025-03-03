#include "TeaCupPuzzleScene.h"

#include "EntityFactory.h"
#include "../src/utils/Vector2D.h"
#include "../src/Components/TriggerComponent.h"
#include "../src/Components/ClickComponent.h"
#include "../src/Components/DragComponent.h"
#include "../src/sdlutils/VirtualTimer.h"
#include "../src/sdlutils/SDLUtils.h"

TeaCupPuzzleScene::TeaCupPuzzleScene()
{
	_spoonIsInCup = false;
	_poisonIsChecked = false;
}

TeaCupPuzzleScene::~TeaCupPuzzleScene()
{

}

void TeaCupPuzzleScene::init()
{
	if (isStarted) return;

	ecs::entity_t teaCupSpoon = entityFactory->CreateInteractableEntity( // Spoon entity
		entityManager, "clockMinArrow", EntityFactory::RECTAREA, 
		Vector2D(100, 400), Vector2D(), 100, 40, 0, 
		areaLayerManager, EntityFactory::DRAG);

	ecs::entity_t teaCup = entityFactory->CreateInteractableEntity( // Cup entity
		entityManager, "clockShape", EntityFactory::RECTAREA,
		Vector2D(400, 400), Vector2D(), 200, 100, 0,
		areaLayerManager, EntityFactory::NODRAG);


	entityManager->getComponent<ClickComponent>(teaCup) // The cup is clicked after introducing the spoon
		->connect(ClickComponent::JUST_CLICKED, [teaCup, this]() 
		{
			if (_spoonIsInCup == false) return;
			_poisonIsChecked = true;

			// ... Change image revealing poinson or whatever  <-- TODO
		});

	entityManager->addComponent<TriggerComponent>(teaCup) // Spoon enters the cup Area2D
		->connect(TriggerComponent::AREA_ENTERED, [this]()
		{
			_spoonIsInCup = true;
		});

	entityManager->getComponent<TriggerComponent>(teaCup) // Spoon leaves the cup Area2D
		->connect(TriggerComponent::AREA_LEFT, [this]()
		{
			_spoonIsInCup = false;
		});

	entityManager->getComponent<DragComponent>(teaCupSpoon) // The spoon is dropped in the cup
		->connect(DragComponent::DRAG_END, [teaCupSpoon, this]()
		{
			if (_spoonIsInCup == false) return;
			entityManager->setActive(teaCupSpoon, false);

			// ... Change image to cup with spoon  <-- TODO
		});

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