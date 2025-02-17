#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

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
		auto _fighter = entityManager->addEntity();
		auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_fighter);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_fighter);
		clk->connect(ClickComponent::JUST_CLICKED, [this]()
			{ 
				std::cout << "CLICKED\n"; 
			});

		TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(_fighter);
		trg->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		trg->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });

		DragComponent* drg = entityManager->addComponent<DragComponent>(_fighter);
		drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });

		
		//iniciar log
		auto _log = entityManager->addEntity();
		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log);

		//add debug lines to log
		logComp->addDialogueLineLog("author1", "hola que tal");
		logComp->addDialogueLineLog("author2", "ahahaha me mato");
		
		logComp->showLog();
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
	
}
