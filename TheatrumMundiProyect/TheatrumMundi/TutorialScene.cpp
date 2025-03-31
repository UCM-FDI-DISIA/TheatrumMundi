#include "TutorialScene.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "LogComponent.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"
#include "ClickableSpriteComponent.h"
#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"



#include "../src/components/WriteTextComponent.h"
#include "DialogueManager.h"


#include "GameSave.h"

TutorialScene::TutorialScene() : SceneRoomTemplate(), _eventToRead("SalaIntermedia1")
{
	dialogueManager = new DialogueManager(1);
	roomEvent.resize(event_size);
	roomEvent[InitialDialogue] = [this]
		{
			startDialogue("SalaIntermedia1");

		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		};
	roomEvent[Spoon] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("TeaCupSpoon", "Es una cuchara, que no lo ves o que", &sdlutils().images().at("TeaCupSpoon")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue("SalaIntermedia1");
		};
	roomEvent[MobileDialogue] = [this] {
		// WIP
		_eventToRead = Movil;
		startDialogue("Movil");
		};

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::init()
{

	if (!isStarted) {
		isStarted = true;
		finishallpuzzles = false;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		Sound puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		a.setVolume(puzzleButtonSound, 0.3);
		//Audio music
		Sound room1music = sdlutils().musics().at("room1music");
		a.setLooping(room1music, true);
		a.playSound(room1music);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);


		auto botonTest = entityManager->addEntity();
		entityManager->addComponent<Transform>(botonTest, Vector2D(1348 / 2.8, 748 / 2), Vector2D(0, 0), 1470 / 4, 270 / 4, 0);
		entityManager->addComponent<Image>(botonTest, &sdlutils().images().at("TutorialButtonTemp"));

		entityManager->addComponent<RectArea2D>(botonTest);

		ClickComponent* clkTut = entityManager->addComponent<ClickComponent>(botonTest);
		clkTut->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			GameSave save;
			save.setTutoCompleted(true);
			save.Write("savegame.dat");
			});


		auto botonBack = entityManager->addEntity();
		entityManager->addComponent<Transform>(botonBack, Vector2D(1348 / 2.5, 748 / 4), Vector2D(0, 0), 1470 / 4, 270 / 4, 0);
		entityManager->addComponent<Image>(botonBack, &sdlutils().images().at("Hanni"));

		entityManager->addComponent<RectArea2D>(botonBack);

		ClickComponent* clkBack = entityManager->addComponent<ClickComponent>(botonBack);
		clkBack->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			});


	}
	SDL_Delay(1000);


}

void TutorialScene::resolvedPuzzle(int i)
{
	/*
	if (i < 4) {
		int auxevent = event_size;
		if (i == 0)  auxevent = PipePuzzleRsv;
		else if (i == 1)  auxevent = BooksPuzzleRsv;
		else if (i == 2)  auxevent = ClockPuzzleRsv;
		else if (i == 3)  auxevent = TeaCupPuzzleRsv;
		roomEvent[auxevent]();
		puzzlesol[i] = true;
		//std::cout << "Puzle resuelto" << i << std::endl;
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		//if (aux) entityManager->setActive(body, true);
	}
	else {
#ifdef _DEBUG
		std::cout << i << " invalid index" << std::endl;
#endif
	}
	*/
}

void TutorialScene::refresh()
{
}

void TutorialScene::unload()
{
	entityManager->~EntityManager();
}
