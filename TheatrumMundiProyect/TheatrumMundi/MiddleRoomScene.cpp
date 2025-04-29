#include "MiddleRoomScene.h"
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
#include "DataManager.h"
#include "EventsInfo.h"
#include "Log.h"

#include "DialogueManager.h"
#include "../src/components/WriteTextComponent.h"
MiddleRoomScene::MiddleRoomScene() :SceneRoomTemplate(), _eventToRead(SalaIntermedia1)
{
	dialogueManager = new DialogueManager(0);
	roomEvent.resize(MIDDLEROOMEVENTSIZE);
	roomEvent[FIRST_DIALOGUE] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM1
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM1_BAD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM2
	roomEvent[AFTER_ROOM2_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_GOOD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_BAD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_BAD1] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM3
	roomEvent[AFTER_ROOM3_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_GOOD2SL] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_GOOD2KL] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD2SK] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD1K] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD1S] = [this]() {
		startDialogue("SalaIntermedia1");
		};
}

MiddleRoomScene::~MiddleRoomScene()
{
	
}

void MiddleRoomScene::init()
{
	if (!isStarted) {
		isStarted = true;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		//Audio music
		//Sound room1music = sdlutils().musics().at("sala1");
		//a.setLooping(room1music, true);
		//a.playSound(room1music);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);
		//MiddleRoomBackground
		entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::MIDDLEROOM);


		//UI
		//Pause
	/*	auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});
*/
		
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);
	}
	SDL_Delay(1000);

	roomEvent[FIRST_DIALOGUE]();
}

void MiddleRoomScene::resolvedPuzzle(int i)
{
}

void MiddleRoomScene::refresh()
{
	int aux = Game::Instance()->getDataManager()->GetActualScene();
	bool auxkei = Game::Instance()->getDataManager()->GetCharacterState(Character::KEISARA);
	bool auxlucy = Game::Instance()->getDataManager()->GetCharacterState(Character::LUCY);
	bool auxsol = Game::Instance()->getDataManager()->GetCharacterState(Character::SOL);
	switch (aux)
	{
	case SceneCount::MIDDLEROOM2:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM1_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM1_BAD2]();
		else {
		#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
		#endif // DEBUG
		}
		break;
	case SceneCount::MIDDLEROOM3:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM2_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM2_GOOD2]();
		else if (auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM2_BAD2]();
		else if (!auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM2_BAD1]();
		else {
		#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
		#endif // DEBUG
		}
		break;
	case SceneCount::END:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM3_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM3_GOOD2SL]();
		else if (auxkei && auxlucy && !auxsol)roomEvent[AFTER_ROOM3_GOOD2KL]();
		else if (auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM3_BAD2SK]();
		else if (!auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM3_BAD1S]();
		else if (auxkei && !auxlucy && !auxsol)roomEvent[AFTER_ROOM3_BAD1K]();
		else {
#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
#endif // DEBUG
		}
		break;
		break;
	default:
	#ifdef _DEBUG
		std::cout << "ERROR INVALID SCENECOUNT";
	#endif // DEBUG

		break;
	}
}

void MiddleRoomScene::unload()
{
}

void MiddleRoomScene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	std::cout << "entro";
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	//Check the act room to load a specific room
	int aux = Game::Instance()->getDataManager()->GetActualScene();
	switch (aux)
	{
	case SceneCount::MIDDLEROOM1:
		Game::Instance()->getSceneManager()->loadScene(ROOM_3);
		break;
	case SceneCount::MIDDLEROOM2:
		Game::Instance()->getSceneManager()->loadScene(ROOM_2);
		break;
	case SceneCount::MIDDLEROOM3:
		Game::Instance()->getSceneManager()->loadScene(ROOM_1);
		break;
	case SceneCount::END:
		//Load the endScene WIP
		//Game::Instance()->getSceneManager()->loadScene(ROOM_1);
		break;
	default:
		break;
	}
	
}
