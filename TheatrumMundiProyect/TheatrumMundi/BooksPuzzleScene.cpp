#include "BooksPuzzleScene.h"

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


using namespace std;

BooksPuzzleScene::BooksPuzzleScene()
{
	comb.resize(10);
	myComb.resize(3);

	for (int i = 0; i < comb.size(); ++i) {
		comb[i] = i;
	}
	for (int i = 0; i < myComb.size(); ++i) {
		myComb[i] = 0;
	}
}

BooksPuzzleScene::~BooksPuzzleScene()
{
}

void BooksPuzzleScene::init()
{
	if (!isStarted) 
	{

		auto backButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(0,0), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG);
		backButton->getMngr()->setActive(backButton, false);

		//auto ButtonBookFirst = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() / 2, sdlutils().height() / 2), Vector2D(0, 0), 50, 300, 0, EntityFactory::NODRAG);
		
		auto ButtonBookFirst = entityManager->addEntity();
		entityManager->addComponent<Transform>(ButtonBookFirst, Vector2D(sdlutils().width()/2, sdlutils().height() / 2), Vector2D(1, 0), 50, 300, 0);
		entityManager->addComponent<Image>(ButtonBookFirst, &sdlutils().images().at("bookButton"));
		entityManager->addComponent<RectArea2D>(ButtonBookFirst,areaLayerManager);
		//else if (_typeRect == CIRCLEAREA) _entityManager->addComponent<CircleArea2D>(newElement)->setLocalPos(Vector2D(_width / 2, _height / 2));
		entityManager->addComponent<ClickComponent>(ButtonBookFirst);
		//entityManager->addComponent<TriggerComponent>(ButtonBookFirst);
		//if (_drag == DRAG) _entityManager->addComponent<DragComponent>(newElement);
		
		auto ButtonBookSecond = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(822, sdlutils().height() / 2), Vector2D(1, 0), 50, 300, 0, areaLayerManager, EntityFactory::NODRAG);
		auto ButtonBookThird = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(900,sdlutils().height() / 2), Vector2D(1, 0), 50, 300, 0, areaLayerManager,EntityFactory::NODRAG);
		auto ImageBook = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(100,100), Vector2D(0, 0), 1200, 600, 0);
		ImageBook->getMngr()->setActive(ImageBook, false);

		ClickComponent* ButtonBook1Click = entityManager->getComponent<ClickComponent>(ButtonBookFirst);
		ButtonBook1Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookA"));
			ImageBook->getMngr()->setActive(ImageBook,true);
			});
		ButtonBook1Click->connect(ClickComponent::JUST_CLICKED, [backButton, this]() {
			backButton->getMngr()->setActive(backButton, true);
			});

		ClickComponent* ButtonBook2Click = entityManager->getComponent<ClickComponent>(ButtonBookSecond);
		ButtonBook2Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookB"));
			ImageBook->getMngr()->setActive(ImageBook, true);
			});
		ButtonBook2Click->connect(ClickComponent::JUST_CLICKED, [backButton, this]() {
			backButton->getMngr()->setActive(backButton, true);
			});

		ClickComponent* ButtonBook3Click = entityManager->getComponent<ClickComponent>(ButtonBookThird);
		ButtonBook3Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookC"));
			ImageBook->getMngr()->setActive(ImageBook, true);
			});
		ButtonBook3Click->connect(ClickComponent::JUST_CLICKED, [backButton, this]() {
			backButton->getMngr()->setActive(backButton, true);
			});

		ClickComponent* clickbackButton = entityManager->getComponent<ClickComponent>(backButton);
		clickbackButton->connect(ClickComponent::JUST_CLICKED, [backButton, this]() {
			backButton->getMngr()->setActive(backButton, false);
		});
		clickbackButton->connect(ClickComponent::JUST_CLICKED, [ImageBook, this]() {
			ImageBook->getMngr()->setActive(ImageBook, false);
		});




		//COMBINATION


		//COMBINATION NUMBER 1
		auto number1 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 100), Vector2D(1, 0), 50, 50, 0);
		auto increaseNumber1 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 180), Vector2D(1, 0), 50, 50, 0, areaLayerManager,EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber1 = entityManager->getComponent<ClickComponent>(increaseNumber1);
		clickIncreaseNumber1->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber1,this]() {
			if (myComb[0] < 9) {
				myComb[0]++;
				cout << "NUM1: " << myComb[0] << endl;
			}
			else if (myComb[0] == 9) {
				myComb[0] = 0;
				cout << "NUM1: " << myComb[0] << endl;
			}
		});

		//COMBINATION NUMBER 2
		auto number2 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0);
		auto increaseNumber2 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, areaLayerManager,EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber2 = entityManager->getComponent<ClickComponent>(increaseNumber2);
		clickIncreaseNumber2->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber2, this]() {
			if (myComb[1] < 9) {
				myComb[1]++;
				cout << "NUM2: " << myComb[1] << endl;
			}
			else if (myComb[1] == 9) {
				myComb[1] = 0;
				cout << "NUM2: " << myComb[1] << endl;
			}
		});

		//COMBINATION NUMBER 3
		auto number3 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0);
		auto increaseNumber3 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, areaLayerManager,EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber3 = entityManager->getComponent<ClickComponent>(increaseNumber3);
		clickIncreaseNumber3->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber3, this]() {
			if (myComb[2] < 9) {
				myComb[2]++;
				cout << "NUM3: " << myComb[2] << endl;
			}
			else if (myComb[2] == 9) {
				myComb[2] = 0;
				cout << "NUM3: " << myComb[2] << endl;
			}
		});

		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 8 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0, areaLayerManager,EntityFactory::NODRAG);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, this]() {
			std::cout << "CLICK" << std::endl;
			if (Check()) std::cout << "WIN" << std::endl;
		});
	}
}

void BooksPuzzleScene::refresh()
{
}

void BooksPuzzleScene::unload()
{
}

bool BooksPuzzleScene::Check()
{ //HERE WE PUT THE CORRECT COMBINATION : 6 - 4 - 1
	return	myComb[0] == comb[6] && 
			myComb[1] == comb[4] && 
			myComb[2] == comb[1];
}
