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
	dialogueManager = new DialogueManager(0);
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

		AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		entityFactory->CreateImageEntity(entityManager, "FondoSalaDeEspera", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
		//INVENTORY
		createInventoryUI();
		

		//where the wires are going to be connected
		ports[0] = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(415, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[1] = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(465, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[2] = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(515, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[3] = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(565, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		ports[4] = entityFactory->CreateInteractableEntity(entityManager, "clockShape", EntityFactory::RECTAREA, Vector2D(615, 555), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		//wires
		wires[0] = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(415, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[1] = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(465, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[2] = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(515, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[3] = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(565, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
		wires[4] = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(615, 295), Vector2D(0, 0), 40, 40, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		//lights
		lights[0] = entityFactory->CreateImageEntity(entityManager, "boa2", Vector2D(300, 295), Vector2D(0, 0), 60, 40, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[1] = entityFactory->CreateImageEntity(entityManager, "boa2", Vector2D(300, 360), Vector2D(0, 0), 60, 40, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[2] = entityFactory->CreateImageEntity(entityManager, "boa2", Vector2D(300, 425), Vector2D(0, 0), 60, 40, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[3] = entityFactory->CreateImageEntity(entityManager, "boa2", Vector2D(300, 480), Vector2D(0, 0), 60, 40, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		lights[4] = entityFactory->CreateImageEntity(entityManager, "boa2", Vector2D(300, 545), Vector2D(0, 0), 60, 40, 0, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

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
		auto checkButton = entityFactory->CreateInteractableEntity(entityManager, "backButton", EntityFactory::RECTAREA, Vector2D(800, 493), Vector2D(0, 0), 70, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);
		ClickComponent* clickcheckButton = entityManager->getComponent<ClickComponent>(checkButton);
		clickcheckButton->connect(ClickComponent::JUST_CLICKED, [checkButton, sr, this, buttonSound]() {

			std::cout << "CLICK" << std::endl;
			AudioManager::Instance().playSound(buttonSound);

			bool allConnected = std::all_of(actualPos.begin(), actualPos.end(), [](int pos) { return pos != -1; });

			if (!allConnected) {
				std::cout << "No todos los cables est�n conectados. El bot�n no hace nada." << std::endl;
				for (int i = 0; i < lights.size(); i++) {
					entityManager->setActive(lights[i], false);
				}
				lightsOn = 0;
				return; // button does nothing if not all cables are connected
			}

			// Check if the combination is correct
			if (Check()) {
				for (int i = 0; i < lights.size(); i++)
				{
					entityManager->setActive(lights[i], true);
				}
				std::cout << "Correct combination" << std::endl;
				Win();
			}
			else
			{
				std::cout << "Incorrect combination" << std::endl;
				std::cout << actualPos[0] << actualPos[1] << actualPos[2] << actualPos[3] << actualPos[4] << std::endl;

				for (int i = 0; i < lightsOn; i++)
				{
					entityManager->setActive(lights[i], true);
				}
			}
			
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
				AudioManager::Instance().playSound(buttonSound);

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
			Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
			areaLayerManager,
			EntityFactory::NODRAG,
			ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
		else gun = entityFactory->CreateInteractableEntity(entityManager, "pistolaSin", EntityFactory::RECTAREA,
			Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
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