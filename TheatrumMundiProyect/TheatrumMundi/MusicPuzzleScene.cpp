#include "MusicPuzzleScene.h"

#include "DialogueManager.h"

#include "../src/utils/Vector2D.h";

#include "EntityFactory.h"

#include <array>

#include "../src/Components/ClickComponent.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/ecs/Manager.h"

#include "../src/components/Image.h"

#include <vector>

#include "iostream"

#include "../src/components/RectArea2D.h"

#include "../src/components/TriggerComponent.h"

#include "../src/Components/Transform.h"

#include "../src/Components/LogComponent.h"

#include "Room1.h"

#include "SceneRoomTemplate.h"

MusicPuzzleScene::MusicPuzzleScene(): _phase(1)
{
    
}

MusicPuzzleScene::~MusicPuzzleScene()
{
}

void MusicPuzzleScene::init(SceneRoomTemplate* sr)
{
    if (!isStarted)
    {
        isStarted = true;
        //Register scene in dialogue manager
        dialogueManager->setScene(this);

        //Puzzle Scene
        room = sr;

        //background + musical notes helpful guide (visual)
        auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "ShelfBackground1", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);


        //piano buttons
        auto number1 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 430), Vector2D(0, 0), /*109, 115*/ 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
        auto number2 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(562, 430), Vector2D(0, 0), /*63, 127*/40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
        auto number3 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(606, 430), Vector2D(0, 0),/* 743, 280*/40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		ClickComponent* clickNumber1 = entityManager->getComponent<ClickComponent>(number1);
		clickNumber1->connect(ClickComponent::JUST_CLICKED, [this, number1]() {
			
			});

		ClickComponent* clickNumber2 = entityManager->getComponent<ClickComponent>(number2);
		clickNumber2->connect(ClickComponent::JUST_CLICKED, [this, number2]() {
			
			});

		ClickComponent* clickNumber3 = entityManager->getComponent<ClickComponent>(number3);
		clickNumber3->connect(ClickComponent::JUST_CLICKED, [this, number3]() {
			
			});

        //musical score (changes in each round)

        //mirror (changes in each round)

        //UI (back button + inventory + log...)
        //BackButton
        auto _backButton = entityManager->addEntity(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
        entityManager->addComponent<Transform>(_backButton, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0);
        entityManager->addComponent<Image>(_backButton, &sdlutils().images().at("B1"));

        entityManager->addComponent<RectArea2D>(_backButton);
    }
}

void MusicPuzzleScene::refresh()
{
}

void MusicPuzzleScene::unload()
{
}

bool MusicPuzzleScene::Check()
{
    return false;
}

void MusicPuzzleScene::Win()
{
}

bool MusicPuzzleScene::checkPhaseCombination()
{
    return false;
}

void MusicPuzzleScene::cleanCombination()
{
}



