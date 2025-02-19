#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"

#include "../src/components/WriteTextComponent.h"

#include "../src/components/LogComponent.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/game/Game.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate()
{
	
}

DebugLogRoom::~DebugLogRoom()
{
}

void DebugLogRoom::init()
{
	if (!isStarted) {

		//All Screen
		auto _screenDetect = entityManager->addEntity();
		auto _screenDetectTr = entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);

		entityManager->addComponent<RectArea2D>(_screenDetect);
	
		ClickComponent* passDialog = entityManager->addComponent<ClickComponent>(_screenDetect);
		passDialog->connect(ClickComponent::JUST_CLICKED, [this]() 
			{
				
				if (!logActive) {
					//read dialogue
					Game::Instance()->getDialogueManager()->ReadDialogue(SalaIntermediaEvento1);
			}
			

			});
		
		//Create dialogue text entity
		auto _textTest = entityManager->addEntity();
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		_textTest->getMngr()->setActive(_textTest, false);

		//Create log
		auto _log = entityManager->addEntity();
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent
		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog")); //background log
		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog = 
			entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("ARIAL24"), colorText, logComp->getLogList()); //write text component
		_log->getMngr()->setActive(_log, false); //hide log at the beggining

		//register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logComp); 

		
		
		//add writeText to dialogueManager
		SDL_Color colorDialog = { 255, 0, 0, 255 }; // Establecer el color (rojo)
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		

		//BUTTONS

		

		//Open log button
		auto _openLogButton = entityManager->addEntity();
		auto _openLogButtonTr = entityManager->addComponent<Transform>(_openLogButton, Vector2D(0, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_openLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_openLogButton);

		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_openLogButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [_log,this]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setActive(_log, true);
				logActive = true;
			});

		//Close log button
		auto _closeLogButton = entityManager->addEntity();
		auto _closeLogButtonTr = entityManager->addComponent<Transform>(_closeLogButton, Vector2D(1000, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_closeLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_closeLogButton);

		ClickComponent* clkClose = entityManager->addComponent<ClickComponent>(_closeLogButton);
		clkClose->connect(ClickComponent::JUST_CLICKED, [_log,this]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setActive(_log, false);
				logActive = false;
			});
		
		//Open dialogue button
		/*auto _openDialogButton = entityManager->addEntity();
		auto _openDialogButtonTr = entityManager->addComponent<Transform>(_openDialogButton, Vector2D(0, 500), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_openDialogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_openDialogButton);

		ClickComponent* clkOpenD = entityManager->addComponent<ClickComponent>(_openDialogButton);
		clkOpenD->connect(ClickComponent::JUST_CLICKED, [_log, _textTest]()
			{
				std::cout << "CLICKED\n";
				_textTest->getMngr()->setActive(_textTest, true);
			});

		//Close dialogue button
		auto _closeDialogButton = entityManager->addEntity();
		auto _closeDialogButtonTr = entityManager->addComponent<Transform>(_closeDialogButton, Vector2D(1000, 500), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_closeDialogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_closeDialogButton);

		ClickComponent* clkCloseD = entityManager->addComponent<ClickComponent>(_closeDialogButton);
		clkCloseD->connect(ClickComponent::JUST_CLICKED, [_log, _textTest]()
			{
				std::cout << "CLICKED\n";
				_textTest->getMngr()->setActive(_textTest, false);
			});
		
		
		*/
		
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
	entityManager->~EntityManager();
}
