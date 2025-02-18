#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "WriteTextComponent.h"

#include "../src/components/LogComponent.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate()
{
	
}

DebugLogRoom::~DebugLogRoom()
{
}

void DebugLogRoom::init()
{
	if (!isStarted) {
		
		//initialize log
		auto _log = entityManager->addEntity();
		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log);

		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0);

		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"));
		

		//add debug lines to log
		logComp->addDialogueLineLog("author1", "hola que tal");
		logComp->addDialogueLineLog("author2", "ahahaha me mato");

		SDL_Color colorText = { 255, 255, 255, 255 };

		WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog = entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("ARIAL24"), colorText, logComp->getLogList());

		_log->getMngr()->setAlive(_log, false);


		//Open log button
		auto _openLogButton = entityManager->addEntity();
		auto _openLogButtonTr = entityManager->addComponent<Transform>(_openLogButton, Vector2D(0, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_openLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_openLogButton);

		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_openLogButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [_log]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setAlive(_log, true);
			});

		//Close log button
		auto _closeLogButton = entityManager->addEntity();
		auto _closeLogButtonTr = entityManager->addComponent<Transform>(_closeLogButton, Vector2D(1000, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_closeLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_closeLogButton);

		ClickComponent* clkClose = entityManager->addComponent<ClickComponent>(_closeLogButton);
		clkClose->connect(ClickComponent::JUST_CLICKED, [_log]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setAlive(_log, false);
			});
		

		
		
		
		
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
}
