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

#include "../src/Components/LogComponent.h"

#include "Room1.h"

#include "SceneRoomTemplate.h"



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

void BooksPuzzleScene::init(SceneRoomTemplate* sr)
{


	if (!isStarted) 
	{
		isStarted = true;
		//Register scene in dialogue manager
		Game::Instance()->getDialogueManager()->setScene(this);

		//CharacterImage
		//auto characterimg = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0, ecs::grp::DIALOGUE);
		auto characterimg = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(characterimg, Vector2D(sdlutils().width() / 5, sdlutils().height() / 8), Vector2D(0, 0), 2019 / 2, 1122 / 2, 0);
		auto imCh = entityManager->addComponent<Image>(characterimg, &sdlutils().images().at("Dialog"));

		Game::Instance()->getDialogueManager()->setCharacterImg(imCh);
		entityManager->setActive(characterimg, false);

		//Create dialogue text entity. Object that renders dialogue Text on Screen
		auto _textbackground = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_textbackground, Vector2D(-200, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
		entityManager->addComponent<RectArea2D>(_textbackground, areaLayerManager);

		entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
			{
				if (!logActive) {
					//read dialogue only if it has to
					if (Game::Instance()->getDialogueManager()->getDisplayOnProcess())
					{
						Game::Instance()->getDialogueManager()->ReadDialogue(Puzzle2);
					}
					else
					{
						_textbackground->getMngr()->setActive(_textbackground, false);
					}
				}
			});
		entityManager->addComponent<TriggerComponent>(_textbackground);
		entityManager->setActive(_textbackground, false);


		auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		entityManager->setActive(_textTest, false);



		//Create log
		auto _log = entityManager->addEntity();
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"), 200); //background log

		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent

		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog =
			entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("BASE"), colorText, logComp->getLogList()); //write text component

		_log->getMngr()->setActive(_log, false); //hide log at the beggining

		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logComp);
		//Game::Instance()->getDialogueManager()->setScene(this);

		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);

		//Game::Instance()->getDialogueManager()->ReadDialogue(Puzzle2);
		startDialogue(Puzzle2);

		

		//Puzzle Scene
		room = sr;
		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "ShelfBackground1", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);

		auto number1 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(518, 430), Vector2D(0, 0), /*109, 115*/ 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto number2 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(562, 430), Vector2D(0, 0), /*63, 127*/40, 40, 0, areaLayerManager, EntityFactory::NODRAG,ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto number3 = entityFactory->CreateInteractableEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(606, 430), Vector2D(0, 0),/* 743, 280*/40, 40, 0, areaLayerManager, EntityFactory::NODRAG,ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		ClickComponent* clickNumber1 = entityManager->getComponent<ClickComponent>(number1);
		clickNumber1->connect(ClickComponent::JUST_CLICKED, [this, number1]() {
			if (myComb[0] < 9) {
				myComb[0]++;
				cout << "NUM1: " << myComb[0] << endl;
			}
			else if (myComb[0] == 9) {
				myComb[0] = 0;
				cout << "NUM1: " << myComb[0] << endl;
			}
			number1->getMngr()->getComponent<Image>(number1)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[0])));
			});

		ClickComponent* clickNumber2 = entityManager->getComponent<ClickComponent>(number2);
		clickNumber2->connect(ClickComponent::JUST_CLICKED, [this, number2]() {
			if (myComb[1] < 9) {
				myComb[1]++;
				cout << "NUM2: " << myComb[1] << endl;
			}
			else if (myComb[1] == 9) {
				myComb[1] = 0;
				cout << "NUM2: " << myComb[1] << endl;
			}
			number2->getMngr()->getComponent<Image>(number2)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[1])));
			});

		ClickComponent* clickNumber3 = entityManager->getComponent<ClickComponent>(number3);
		clickNumber3->connect(ClickComponent::JUST_CLICKED, [this, number3]() {
			if (myComb[2] < 9) {
				myComb[2]++;
				cout << "NUM3: " << myComb[2] << endl;
			}
			else if (myComb[2] == 9) {
				myComb[2] = 0;
				cout << "NUM3: " << myComb[2] << endl;
			}
			number3->getMngr()->getComponent<Image>(number3)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[2])));
			});

	////COMBINATION
	//
	////COMBINATION NUMBER 1
	//	auto number1 = entityFactory->CreateImageEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	auto increaseNumber1 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	ClickComponent* clickIncreaseNumber1 = entityManager->getComponent<ClickComponent>(increaseNumber1);
	//	clickIncreaseNumber1->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber1, number1, this]() {
	//		if (myComb[0] < 9) {
	//			myComb[0]++;
	//			cout << "NUM1: " << myComb[0] << endl;
	//		}
	//		else if (myComb[0] == 9) {
	//			myComb[0] = 0;
	//			cout << "NUM1: " << myComb[0] << endl;
	//		}
	//		number1->getMngr()->getComponent<Image>(number1)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[0])));
	//		});
	//
	//	//COMBINATION NUMBER 2
	//	auto number2 = entityFactory->CreateImageEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	auto increaseNumber2 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	ClickComponent* clickIncreaseNumber2 = entityManager->getComponent<ClickComponent>(increaseNumber2);
	//	clickIncreaseNumber2->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber2, number2, this]() {
	//		if (myComb[1] < 9) {
	//			myComb[1]++;
	//			cout << "NUM2: " << myComb[1] << endl;
	//		}
	//		else if (myComb[1] == 9) {
	//			myComb[1] = 0;
	//			cout << "NUM2: " << myComb[1] << endl;
	//		}
	//		number2->getMngr()->getComponent<Image>(number2)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[1])));
	//		});
	//
	//	//COMBINATION NUMBER 3
	//	auto number3 = entityFactory->CreateImageEntity(entityManager, "bookComb0", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 100), Vector2D(0, 0), 50, 50, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	auto increaseNumber3 = entityFactory->CreateInteractableEntity(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() - 180), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
	//	ClickComponent* clickIncreaseNumber3 = entityManager->getComponent<ClickComponent>(increaseNumber3);
	//	clickIncreaseNumber3->connect(ClickComponent::JUST_CLICKED, [clickIncreaseNumber3, number3, this]() {
	//		if (myComb[2] < 9) {
	//			myComb[2]++;
	//			cout << "NUM3: " << myComb[2] << endl;
	//		}
	//		else if (myComb[2] == 9) {
	//			myComb[2] = 0;
	//			cout << "NUM3: " << myComb[2] << endl;
	//		}
	//		number3->getMngr()->getComponent<Image>(number3)->setTexture(&sdlutils().images().at("bookComb" + std::to_string(myComb[2])));
	//		});

		//REWARD ENTITY
		auto Reward = entityFactory->CreateImageEntity(entityManager, "prueba", Vector2D(510, 548), Vector2D(0, 0), 340, 200, 0, ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
		Reward->getMngr()->setActive(Reward, false);																																					

		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(700,480), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, Reward, this]() {
			std::cout << "CLICK" << std::endl;
			if (Check()) {
				std::cout << "WIN" << std::endl;
				//Reward->getMngr()->setActive(Reward, true);
				Reward->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, true);
			}
			});

		//BOOKS
		auto backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20,20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK);
		backButton->getMngr()->setActive(backButton, false);

		auto ButtonBookFirst = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook1", EntityFactory::RECTAREA, Vector2D(620, 0), Vector2D(0, 0), 85, 110, 0,areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto ButtonBookSecond = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook2", EntityFactory::RECTAREA, Vector2D(515, 145), Vector2D(0, 0), 85, 110, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto ButtonBookThird = entityFactory->CreateInteractableEntity(entityManager, "ShelfBook3", EntityFactory::RECTAREA, Vector2D(750,280), Vector2D(0, 0), 85, 110, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		auto ImageBook = entityFactory->CreateImageEntity(entityManager, "bookA", Vector2D(100,100), Vector2D(0, 0), 1200, 600, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK);
		ImageBook->getMngr()->setActive(ImageBook, false);

		//ClickComponent* ButtonBook1Click = entityManager->getComponent<ClickComponent>(ButtonBookFirst);
		//ButtonBook1Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, increaseNumber1, increaseNumber2, increaseNumber3, backButton, checkButton, this]() {
		//	ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookA"));
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
		//	});
		//ClickComponent* ButtonBook2Click = entityManager->getComponent<ClickComponent>(ButtonBookSecond);
		//ButtonBook2Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, increaseNumber1, increaseNumber2, increaseNumber3, backButton, checkButton, this]() {
		//	ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookB"));
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
		//	});
		//ClickComponent* ButtonBook3Click = entityManager->getComponent<ClickComponent>(ButtonBookThird);
		//ButtonBook3Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, increaseNumber1, increaseNumber2, increaseNumber3, backButton, checkButton,this]() {
		//	ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookC"));
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
		//	});
		//ClickComponent* clickbackButton = entityManager->getComponent<ClickComponent>(backButton);
		//clickbackButton->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, increaseNumber1, increaseNumber2, increaseNumber3, backButton, checkButton,this]() {
		//	ImageBook->getMngr()->setActive(ImageBook, false);
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, false);
		//	ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, true);
		//});

		ClickComponent* ButtonBook1Click = entityManager->getComponent<ClickComponent>(ButtonBookFirst);
		ButtonBook1Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, backButton, checkButton, Reward, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookA"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			Reward->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* ButtonBook2Click = entityManager->getComponent<ClickComponent>(ButtonBookSecond);
		ButtonBook2Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, backButton, checkButton, Reward, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookB"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			Reward->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* ButtonBook3Click = entityManager->getComponent<ClickComponent>(ButtonBookThird);
		ButtonBook3Click->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, backButton, checkButton, Reward, this]() {
			ImageBook->getMngr()->getComponent<Image>(ImageBook)->setTexture(&sdlutils().images().at("bookC"));
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, true);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, false);
			Reward->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, false);
			});

		ClickComponent* clickbackButton = entityManager->getComponent<ClickComponent>(backButton);
		clickbackButton->connect(ClickComponent::JUST_CLICKED, [ImageBook, ButtonBookFirst, ButtonBookSecond, ButtonBookThird, backButton, checkButton, Reward, this]() {
			ImageBook->getMngr()->setActive(ImageBook, false);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_BOOK, false);
			ImageBook->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL, true);
			Reward->getMngr()->setActiveGroup(ecs::grp::BOOKS_PUZZLE_SCENE_REWARD, true);
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

void BooksPuzzleScene::refresh()
{
}

void BooksPuzzleScene::unload()
{
	
}

bool BooksPuzzleScene::Check()
{ //HERE WE PUT THE CORRECT COMBINATION : 6 - 4 - 1
	//CORRECT ACTUAL COMBINATION: 1 - 6 - 4
	return	myComb[0] == comb[1] && 
			myComb[1] == comb[6] && 
			myComb[2] == comb[4];
}

void BooksPuzzleScene::Win()
{
	room->resolvedPuzzle(5);
	Game::Instance()->getSceneManager()->popScene();
}


