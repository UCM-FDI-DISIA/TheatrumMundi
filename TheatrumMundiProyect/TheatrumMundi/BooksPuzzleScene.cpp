#include "BooksPuzzleScene.h"

#include "../src/utils/Vector2D.h";

#include "EntityFactory.h"

#include <array>

#include "../src/Components/ClickComponent.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/ecs/Manager.h"

#include "../src/components/Image.h"


using namespace std;

BooksPuzzleScene::BooksPuzzleScene()
{
	for (int i = 0; i < comb1.size(); ++i) {
		comb1[i] = i;
		comb2[i] = i;
		comb3[i] = i;
	}

	num1 = 0;
	num2 = 0;
	num3 = 0;
}

BooksPuzzleScene::~BooksPuzzleScene()
{
}

void BooksPuzzleScene::init()
{
	if (!isStarted) 
	{

		auto backButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(0,0), Vector2D(0, 0), 50, 50, 0, EntityFactory::NODRAG);
		backButton->getMngr()->setActive(backButton, false);

		auto ButtonBookFirst = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(274, sdlutils().height() / 2), Vector2D(0, 0), 50, 300, 0, EntityFactory::NODRAG);
		auto ButtonBookSecond = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(822, sdlutils().height() / 2), Vector2D(0, 0), 50, 300, 0, EntityFactory::NODRAG);
		auto ButtonBookThird = entityFactory->CreateInteractableEntity(entityManager, "bookButton", EntityFactory::RECTAREA, Vector2D(900,sdlutils().height() / 2), Vector2D(0, 0), 50, 300, 0, EntityFactory::NODRAG);
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
		auto number1 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0);
		auto increaseNumber1 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber1 = entityManager->getComponent<ClickComponent>(increaseNumber1);
		clickIncreaseNumber1->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber1,this]() {
			if (num1 < 9) {
				num1++;
				cout << "NUM1: " << num1 << endl;
			}
			else if (num1 == 9) {
				num1 = 0;
				cout << "NUM1: " << num1 << endl;
			}
		});

		//COMBINATION NUMBER 2
		auto number2 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0);
		auto increaseNumber2 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber2 = entityManager->getComponent<ClickComponent>(increaseNumber2);
		clickIncreaseNumber2->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber2, this]() {
			if (num2 < 9) {
				num2++;
				cout << "NUM2: " << num2 << endl;
			}
			else if (num2 == 9) {
				num2 = 0;
				cout << "NUM2: " << num2 << endl;
			}
		});

		//COMBINATION NUMBER 3
		auto number3 = entityFactory->CreateImageEntity(entityManager, "numberButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0);
		auto increaseNumber3 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, EntityFactory::NODRAG);
		ClickComponent* clickIncreaseNumber3 = entityManager->getComponent<ClickComponent>(increaseNumber3);
		clickIncreaseNumber3->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber3, this]() {
			if (num3 < 9) {
				num3++;
				cout << "NUM3: " << num3 << endl;
			}
			else if (num3 == 9) {
				num3 = 0;
				cout << "NUM3: " << num3 << endl;
			}
		});

		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 8 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0, EntityFactory::NODRAG);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, this]() {
			if (Check()) std::cout << "WIN" << std::endl;
			std::cout << "CLICK" << std::endl;
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
{
	return comb1[2] == num1 && comb2[7] == num2 && comb3[3] == num3;
}
