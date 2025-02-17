#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "Log.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate()
{
	_sceneLog = new Log();
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
				_sceneLog->showLog();
			});

		TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(_fighter);
		trg->connect(TriggerComponent::JUST_ENTERED, []() { std::cout << "ENTERED\n";  });
		trg->connect(TriggerComponent::JUST_LEFT, []() { std::cout << "LEFT\n";  });

		DragComponent* drg = entityManager->addComponent<DragComponent>(_fighter);
		drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });


		//add debug lines to log
		_sceneLog->addDialogueLineLog("ev1", { "authorX", "hola que tal" });
		_sceneLog->addDialogueLineLog("ev1", { "authorY", "aaaa" });
		_sceneLog->addDialogueLineLog("ev1", { "authorY", "/" });

		_sceneLog->addDialogueLineLog("ev2", { "authorX", "KOKOKO" });
		_sceneLog->addDialogueLineLog("ev2", { "authorX", "YHYHY" });
		_sceneLog->addDialogueLineLog("ev1", { "authorY", "/" });

		_sceneLog->addDialogueLineLog("ev1", { "authorX", "hola que tal" });
		_sceneLog->addDialogueLineLog("ev1", { "authorY", "aaaa" });
		_sceneLog->addDialogueLineLog("ev1", { "authorY", "/" });

		
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
	delete _sceneLog;
}
