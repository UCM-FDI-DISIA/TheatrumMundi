#include "Room1.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"


Room1Scene::Room1Scene(): SceneRoomTemplate()
{
	roomEvent.resize(event_size);
	roomEvent[InitialDialogue] = [this] 
	{
		startDialogue(SalaIntermediaEvento1);
	};
	roomEvent[CorpseDialogue] = [this]
		{
			startDialogue(SalaIntermediaEvento1);
		};
	roomEvent[PipePuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(PipePuzzleSnc,this);
		};
	roomEvent[PipePuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(0);
		};
	roomEvent[BooksPuzzleScn] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(BooksPuzzleScn, this);
		};
	roomEvent[BooksPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(1);
		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(ClockPuzzleSnc, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		resolvedPuzzle(2);
		};
	roomEvent[TeaCupPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(TeaCupPuzzleSnc, this);
		};
	roomEvent[TeaCupPuzzleRsv] = [this] {
		// InventoryLogic
		entityManager->removeComponent<ClickComponent>(puzzleptr[4]);
		};
	roomEvent[Spoon] = [this] {
		// InventoryLogic
		};
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue(SalaIntermediaEvento1);
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		};
	roomEvent[Log] = [this] {

		};
}

Room1Scene::~Room1Scene()
{
}

void Room1Scene::init()
{
	
	if (!isStarted) {

		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(253, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 1, 1, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		
		
		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		auto StudyBackgroundScroll = entityManager->getComponent<ScrollComponent>(StudyBackground);
		auto StudyBackgroundTransform = entityManager->getComponent<Transform>(StudyBackground);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyBackground));

		auto LivingBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(0 - sdlutils().width(), 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		auto LivingBackgroundScroll = entityManager->getComponent<ScrollComponent>(LivingBackground);
		LivingBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingBackground));

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1Button,StudyBackgroundScroll]() {

			if (!StudyBackgroundScroll->isScrolling()) {

				StudyBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
			

			});
		
	}
	//SDL_Delay(1000);
	//_loadimg->getMngr()->setActive(_loadimg,false);
	
}

void Room1Scene::resolvedPuzzle(int i)
{
	if (i == ClockPuzzleRsv || i == PipePuzzleRsv || i == BooksPuzzleRsv || i == TeaCupPuzzleRsv) {
		roomEvent[i]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		if (aux) entityManager->setActive(body, true);
	}
else {
#ifdef _DEBUG
	std::cout << i << " invalid index" << std::endl;
#endif
}
}

void Room1Scene::refresh()
{
}

void Room1Scene::unload()
{
	entityManager->~EntityManager();
}
