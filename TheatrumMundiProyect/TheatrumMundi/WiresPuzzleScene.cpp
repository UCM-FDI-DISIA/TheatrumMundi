#include "WiresPuzzleScene.h"
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

#include "DialogueManager.h"
#include "../src/game/Game.h"
#include "Log.h"

using namespace std;

WiresPuzzleScene::WiresPuzzleScene() : lightsOn(0), selectedWireIndex(-1)
{
	dialogueManager = new DialogueManager(5);
}

WiresPuzzleScene::~WiresPuzzleScene()
{
}

void WiresPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted)
	{	//Puzzle scene
		room = sr;
		isStarted = true;
		dialogueManager->setScene(this);

		//inicialize the vector with -1 and the size of wires and ports
		cableToPort.resize(wires.size(), -1);
		portToCable.resize(ports.size(), -1);

		//	AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		audioManager().setVolume(buttonSound, 0.2);
		entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);

		entityFactory->CreateImageEntity(entityManager, "cajaFinal", Vector2D(50, 100), Vector2D(0, 0), 854, 636, 0, ecs::grp::DEFAULT);

		entityFactory->CreateImageEntity(entityManager, "recompensa", Vector2D(900, 120), Vector2D(0, 0), 383, 602, 0, ecs::grp::DEFAULT);

		//cables que se ven
		cablesCortados[0] = entityFactory->CreateImageEntity(entityManager, "cable61", Vector2D(330, 170), Vector2D(0, 0), 62, 245, 0, ecs::grp::DEFAULT);
		cablesCortados[1] = entityFactory->CreateImageEntity(entityManager, "cable62", Vector2D(415, 170), Vector2D(0, 0), 61, 267, 0, ecs::grp::DEFAULT);
		cablesCortados[2] = entityFactory->CreateImageEntity(entityManager, "cable63", Vector2D(515, 170), Vector2D(0, 0), 64, 263, 0, ecs::grp::DEFAULT);
		cablesCortados[3] = entityFactory->CreateImageEntity(entityManager, "cable64", Vector2D(620, 170), Vector2D(0, 0), 50, 220, 0, ecs::grp::DEFAULT);
		cablesCortados[4] = entityFactory->CreateImageEntity(entityManager, "cable65", Vector2D(720, 170), Vector2D(0, 0), 50, 213, 0, ecs::grp::DEFAULT);

		//cables al conectar iguales
		cablesVisibles1[0] = entityFactory->CreateImageEntity(entityManager, "cable11", Vector2D(300, 190), Vector2D(0, 0), 108, 476, 0, ecs::grp::DEFAULT);
		cablesVisibles1[1] = entityFactory->CreateImageEntity(entityManager, "cable12", Vector2D(395, 190), Vector2D(0, 0), 109, 483, 0, ecs::grp::DEFAULT);
		cablesVisibles1[2] = entityFactory->CreateImageEntity(entityManager, "cable13", Vector2D(485, 190), Vector2D(0, 0), 108, 477, 0, ecs::grp::DEFAULT);
		cablesVisibles1[3] = entityFactory->CreateImageEntity(entityManager, "cable14", Vector2D(590, 190), Vector2D(0, 0), 110, 483, 0, ecs::grp::DEFAULT);
		cablesVisibles1[4] = entityFactory->CreateImageEntity(entityManager, "cable15", Vector2D(690, 190), Vector2D(0, 0), 115, 487, 0, ecs::grp::DEFAULT);


		for (int i = 0; i < 5; ++i) {
			entityManager->setActive(cablesVisibles1[i], false);
		}

		//cables al conectar +1
		cablesVisibles2Pos[0] = entityFactory->CreateImageEntity(entityManager, "cable21", Vector2D(320, 190), Vector2D(0, 0), 148, 488, 0, ecs::grp::DEFAULT);
		cablesVisibles2Pos[1] = entityFactory->CreateImageEntity(entityManager, "cable22", Vector2D(425, 190), Vector2D(0, 0), 140, 476, 0, ecs::grp::DEFAULT);
		cablesVisibles2Pos[2] = entityFactory->CreateImageEntity(entityManager, "cable23", Vector2D(525, 190), Vector2D(0, 0), 139, 481, 0, ecs::grp::DEFAULT);
		cablesVisibles2Pos[3] = entityFactory->CreateImageEntity(entityManager, "cable24", Vector2D(610, 190), Vector2D(0, 0), 153, 473, 0, ecs::grp::DEFAULT);

		for (int i = 0; i < 4; ++i) {
			entityManager->setActive(cablesVisibles2Pos[i], false);
		}

		//cables al conectar -1

		cablesVisibles2Neg[3] = entityFactory->CreateImageEntity(entityManager, "cable25", Vector2D(630, 190), Vector2D(0, 0), 140, 490, 0, ecs::grp::DEFAULT);
		cablesVisibles2Neg[2] = entityFactory->CreateImageEntity(entityManager, "cable26", Vector2D(530, 190), Vector2D(0, 0), 125, 471, 0, ecs::grp::DEFAULT);
		cablesVisibles2Neg[1] = entityFactory->CreateImageEntity(entityManager, "cable27", Vector2D(435, 190), Vector2D(0, 0), 132, 471, 0, ecs::grp::DEFAULT);
		cablesVisibles2Neg[0] = entityFactory->CreateImageEntity(entityManager, "cable28", Vector2D(305, 190), Vector2D(0, 0), 176, 490, 0, ecs::grp::DEFAULT);


		for (int i = 0; i < 4; ++i) {
			entityManager->setActive(cablesVisibles2Neg[i], false);
		}

		//cables al conectar +2 
		cablesVisibles3Pos[0] = entityFactory->CreateImageEntity(entityManager, "cable31", Vector2D(340, 190), Vector2D(0, 0), 245, 490, 0, ecs::grp::DEFAULT);
		cablesVisibles3Pos[1] = entityFactory->CreateImageEntity(entityManager, "cable32", Vector2D(435, 190), Vector2D(0, 0), 220, 485, 0, ecs::grp::DEFAULT);
		cablesVisibles3Pos[2] = entityFactory->CreateImageEntity(entityManager, "cable33", Vector2D(535, 190), Vector2D(0, 0), 245, 474, 0, ecs::grp::DEFAULT);

		for (int i = 0; i < 3; ++i) {
			entityManager->setActive(cablesVisibles3Pos[i], false);
		}

		//cables al conectar -2 
		cablesVisibles3Neg[2] = entityFactory->CreateImageEntity(entityManager, "cable34", Vector2D(535, 190), Vector2D(0, 0), 220, 485, 0, ecs::grp::DEFAULT);
		cablesVisibles3Neg[1] = entityFactory->CreateImageEntity(entityManager, "cable35", Vector2D(435, 190), Vector2D(0, 0), 223, 480, 0, ecs::grp::DEFAULT);
		cablesVisibles3Neg[0] = entityFactory->CreateImageEntity(entityManager, "cable36", Vector2D(340, 190), Vector2D(0, 0), 221, 478, 0, ecs::grp::DEFAULT);

		for (int i = 0; i < 3; ++i) {
			entityManager->setActive(cablesVisibles3Neg[i], false);
		}

		//cables al conectar +3 
		cablesVisibles4Pos[0] = entityFactory->CreateImageEntity(entityManager, "cable41", Vector2D(340, 190), Vector2D(0, 0), 333, 473, 0, ecs::grp::DEFAULT);
		cablesVisibles4Pos[1] = entityFactory->CreateImageEntity(entityManager, "cable42", Vector2D(435, 190), Vector2D(0, 0), 325, 478, 0, ecs::grp::DEFAULT);
		for (int i = 0; i < 2; ++i) {
			entityManager->setActive(cablesVisibles4Pos[i], false);
		}

		//cables al conectar -3 
		cablesVisibles4Neg[0] = entityFactory->CreateImageEntity(entityManager, "cable43", Vector2D(340, 190), Vector2D(0, 0), 328, 469, 0, ecs::grp::DEFAULT);
		cablesVisibles4Neg[1] = entityFactory->CreateImageEntity(entityManager, "cable44", Vector2D(435, 190), Vector2D(0, 0), 326, 472, 0, ecs::grp::DEFAULT);

		for (int i = 0; i < 2; ++i) {
			entityManager->setActive(cablesVisibles4Neg[i], false);
		}

		//cable al conectar +4
		cableVisible5Pos = entityFactory->CreateImageEntity(entityManager, "cable51", Vector2D(340, 190), Vector2D(0, 0), 431, 479, 0, ecs::grp::DEFAULT);
		entityManager->setActive(cableVisible5Pos, false);
		// 
			//cable al conectar -4
		cableVisible5Neg = entityFactory->CreateImageEntity(entityManager, "cable52", Vector2D(340, 190), Vector2D(0, 0), 420, 479, 0, ecs::grp::DEFAULT);
		entityManager->setActive(cableVisible5Neg, false);


		entityFactory->CreateImageEntity(entityManager, "estCables", Vector2D(230, 130), Vector2D(0, 0), 640, 593, 0, ecs::grp::DEFAULT);


		//where the wires are going to be connected
		ports[0] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(340, 650), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[1] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(440, 650), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[2] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(540, 650), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[3] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(630, 650), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[4] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(730, 650), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		//wires
		wires[0] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(340, 170), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[1] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(440, 170), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[2] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(540, 170), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[3] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(630, 170), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[4] = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(730, 170), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		//lights
		lights[0] = entityFactory->CreateImageEntity(entityManager, "luzOn", Vector2D(220, 295), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[1] = entityFactory->CreateImageEntity(entityManager, "luzOn", Vector2D(220, 360), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[2] = entityFactory->CreateImageEntity(entityManager, "luzOn", Vector2D(220, 425), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[3] = entityFactory->CreateImageEntity(entityManager, "luzOn", Vector2D(220, 480), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[4] = entityFactory->CreateImageEntity(entityManager, "luzOn", Vector2D(220, 545), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

		for (int i = 0;i < 5; ++i) {
			entityManager->setActive(lights[i], false);
		}

		offLights[0] = entityFactory->CreateImageEntity(entityManager, "luzOff", Vector2D(220, 295), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		offLights[1] = entityFactory->CreateImageEntity(entityManager, "luzOff", Vector2D(220, 360), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		offLights[2] = entityFactory->CreateImageEntity(entityManager, "luzOff", Vector2D(220, 425), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		offLights[3] = entityFactory->CreateImageEntity(entityManager, "luzOff", Vector2D(220, 480), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		offLights[4] = entityFactory->CreateImageEntity(entityManager, "luzOff", Vector2D(220, 545), Vector2D(0, 0), 49, 49, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);







		//deactivate the lights
		for (int i = 0; i < lights.size(); i++) {
			entityManager->setActive(lights[i], false);
		}


		//pressed wires
		for (int i = 0; i < wires.size(); i++) {
			entityManager->addComponent<ClickComponent>(wires[i]);

			wires[i]->getMngr()->getComponent<ClickComponent>(wires[i])->connect(ClickComponent::JUST_CLICKED, [this, i]() {
				if (selectedWireIndex == i) {
					// unselect the wire
					selectedWireIndex = -1;
					std::cout << "Cable " << i << " deseleccionado" << std::endl;
					// quitar borde visual
				}
				else {
					selectedWireIndex = i;
					std::cout << "Cable " << i << " seleccionado" << std::endl;
					// borde visual
				}
				});
		}

		//pressed ports
		for (int i = 0; i < ports.size(); i++) {
			entityManager->addComponent<ClickComponent>(ports[i]);

			ports[i]->getMngr()->getComponent<ClickComponent>(ports[i])->connect(ClickComponent::JUST_CLICKED, [this, i]() {
				if (selectedWireIndex != -1) {
					int wireIndex = selectedWireIndex;

					// if the cable is already connected to another port, we need to disconnect it first
					if (portToCable[i] != -1) {
						int otherWire = portToCable[i];
						cableToPort[otherWire] = -1;
						entityManager->setActive(wires[otherWire], true); // move the old cable to its original position
						std::cout << "Cable " << otherWire << " desconectado del puerto " << i << std::endl;

						actualPos[otherWire] = -1; // set the position of the old cable to -1
					}

					//conect the cable to the port selected
					cableToPort[wireIndex] = i;
					portToCable[i] = wireIndex;

					//deactivate the cable
					entityManager->setActive(wires[wireIndex], false);
					entityManager->setActive(ports[i], false);
					entityManager->setActive(cablesCortados[wireIndex], false);

					if (i == wireIndex) {
						entityManager->setActive(cablesVisibles1[wireIndex], true);
					}
					if (i - 1 == wireIndex) {
						entityManager->setActive(cablesVisibles2Pos[wireIndex], true);
					}
					if (i + 1 == wireIndex) {
						entityManager->setActive(cablesVisibles2Neg[i], true);
					}
					if (i - 2 == wireIndex) {
						entityManager->setActive(cablesVisibles3Pos[wireIndex], true);
					}
					if (i + 2 == wireIndex) {
						entityManager->setActive(cablesVisibles3Neg[i], true);
					}
					if (i - 3 == wireIndex) {
						entityManager->setActive(cablesVisibles4Pos[wireIndex], true);
					}
					if (i + 3 == wireIndex) {
						entityManager->setActive(cablesVisibles4Neg[i], true);
					}
					if (i - 4 == wireIndex) {
						entityManager->setActive(cableVisible5Pos, true);
					}
					if (i + 4 == wireIndex) {
						entityManager->setActive(cableVisible5Neg, true);
					}

					auto cable = "cable" + (wireIndex + 1) + (i + 1);

					//asign the position of the cable
					actualPos[wireIndex] = i;

					std::cout << "Cable " << wireIndex << " conectado al puerto " << i << std::endl;

					//unselect the wire
					selectedWireIndex = -1;

					//light
				}
				else if (selectedWireIndex == -1 && portToCable[i] != -1)
				{
					int otherWire = portToCable[i];
					cableToPort[otherWire] = -1; //desconnect the cable
					portToCable[i] = -1; //desconnect the port
					entityManager->setActive(wires[otherWire], true); // move the cable to its original position
					std::cout << "Cable " << otherWire << " desconectado del puerto " << i << std::endl;

					actualPos[otherWire] = -1; // set the position of the old cable to -1
				}
				else {
					actualPos[i] = -1; // if the port is not connected to any cable, set the position to -1
				}

				});
		}


		//CHECK COMBINATION
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "botonUsar", EntityFactory::RECTAREA, Vector2D(100, 150), Vector2D(0, 0), 110, 121, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, sr, this, buttonSound]() {

			std::cout << "CLICK" << std::endl;
			audioManager().playSound(buttonSound);

			bool allConnected = std::all_of(actualPos.begin(), actualPos.end(), [](int pos) { return pos != -1; });

			if (!allConnected) {
				std::cout << "No todos los cables est�n conectados. El bot�n no hace nada." << std::endl;
				for (int i = 0; i < lights.size(); i++) {
					entityManager->setActive(lights[i], false);
					entityManager->setActive(offLights[i], true);
				}
				lightsOn = 0;
				return; // button does nothing if not all cables are connected
			}

			// Check if the combination is correct
			if (Check()) {
				for (int i = 0; i < lights.size(); i++)
				{
					entityManager->setActive(lights[i], true);
					entityManager->setActive(offLights[i], false);
				}
				std::cout << "Correct combination" << std::endl;
				entityManager->setActive(resetButton, false);
				Win();
			}
			else
			{
				std::cout << "Incorrect combination" << std::endl;
				std::cout << actualPos[0] << actualPos[1] << actualPos[2] << actualPos[3] << actualPos[4] << std::endl;

				for (int i = 0; i < lightsOn; i++)
				{
					entityManager->setActive(lights[i], true);
					entityManager->setActive(offLights[i], false);
				}
			}

			});

		//reset button
		 resetButton = entityFactory->CreateInteractableEntity(entityManager, "resetButton", EntityFactory::RECTAREA, Vector2D(100, 350), Vector2D(0, 0), 110, 121, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickresetButton = entityManager->getComponent<ClickComponent>(resetButton);
		clickresetButton->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			audioManager().playSound(buttonSound);
			desconectarCables();
			});
	

		//BackButton
		//ENTIDADCONENTITYFACTORY
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);


		//INVENTORY
		//Invntory Background
		createInventoryUI();
		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton, buttonSound]()
			{
				audioManager().playSound(buttonSound);

				inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
				HideInventoryItems();
				room->GetInventory()->setFirstItem(0);
				auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
				_backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
				_backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
				_backButtonImage->setPosOffset(0, 0);
				Game::Instance()->getSceneManager()->popScene();
			});
		dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
		dialogueManager->setScene(this);

		//Rewards
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(2);
		
		if (variant == 2)gun = entityFactory->CreateInteractableEntity(entityManager, "pistolaCon", EntityFactory::RECTAREA,
			Vector2D(1050, 620), Vector2D(0, 0), 220, 220, 0,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
		
		
		else gun = entityFactory->CreateInteractableEntity(entityManager, "pistolaSin", EntityFactory::RECTAREA,
			Vector2D(1050, 620), Vector2D(0, 0), 220, 220, 0,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
		ClickComponent* clk = entityManager->getComponent<ClickComponent>(gun);
		clk->connect(ClickComponent::JUST_CLICKED, [this, variant, sr]() {

			Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
			if (variant == 2)AddInvItem("pistolaCon", sdlutils().Instance()->invDescriptions().at("PistolaCon"), position, sr);
			else  AddInvItem("pistolaSin", sdlutils().Instance()->invDescriptions().at("PistolaSin"), position, sr);
			gun->getMngr()->setActive(gun, false);

			});
		entityManager->setActive(gun, false);
		dialogueManager->setScene(this);
		if(Game::Instance()->getDataManager()->GetCharacterState(SOL)&& Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("CABLES_2P");
		else {
			if(Game::Instance()->getDataManager()->GetCharacterState(SOL))startDialogue("CABLES_1PS");
			else if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("CABLES_1PK");
		}



	}
	//IMPORTANT this need to be out of the isstarted!!!
	sr->GetInventory()->setFirstItem(0);
	createInvEntities(sr);
}

void WiresPuzzleScene::unload()
{

}

bool WiresPuzzleScene::Check()
{ 	//CORRECT ACTUAL COMBINATION: 2 - 0 - 4 - 1 - 3

	lightsOn = 0;

	for (auto light : lights) {
		entityManager->setActive(light, false);
	}

	for (int i = 0; i < actualPos.size(); i++)
	{
		if (actualPos[i] == winPos[i]) //verify that the wires are in the correct position
		{
			lightsOn++;
		}
	}

	if (lightsOn == 5)
	{
		solved = true;
		return true;
	}
	else
	{
		return false;
	}
	
}
void WiresPuzzleScene::Win()
{
	entityManager->setActive(gun, true);
	room->resolvedPuzzle(1);
}

void WiresPuzzleScene::ResolveScene()
{
	Win();
}

void WiresPuzzleScene::desconectarCables()
{
	for (int i = 0; i < 5; ++i) {
		entityManager->setActive(cablesVisibles1[i], false);
	}

	for (int i = 0; i < 4; ++i) {
		entityManager->setActive(cablesVisibles2Pos[i], false);
		entityManager->setActive(cablesVisibles2Neg[i], false);
	}
	
	for (int i = 0; i < 3; ++i) {
		entityManager->setActive(cablesVisibles3Pos[i], false);
		entityManager->setActive(cablesVisibles3Neg[i], false);
	}

	for (int i = 0; i < 2; ++i) {
		entityManager->setActive(cablesVisibles4Pos[i], false);
		entityManager->setActive(cablesVisibles4Neg[i], false);
	}
	
	entityManager->setActive(cableVisible5Pos, false);
	entityManager->setActive(cableVisible5Neg, false);


	for (int i = 0; i < 5; ++i) {
		entityManager->setActive(cablesCortados[i], true);
		entityManager->setActive(wires[i], true);
		entityManager->setActive(ports[i], true);
	}

	actualPos = { -1, -1, -1, -1, -1 };
	
	for (int i = 0; i < 5; ++i) {
		cableToPort[i] = -1;
		portToCable[i] = -1;
	}
}
