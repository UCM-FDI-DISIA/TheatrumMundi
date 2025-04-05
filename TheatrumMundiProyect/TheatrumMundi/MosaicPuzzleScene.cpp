#include "MosaicPuzzleScene.h"
#include "PhysicsBodyComponent.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "RectArea2D.h"
#include "ClickableSpriteComponent.h"
#include "SDLUtils.h"
#include "Image.h"
#include "DialogueManager.h"
#include "Log.h"
#include "TriggerComponent.h"
#include "DragComponent.h"

/// <summary>
/// Create the dragSquares
/// </summary>
void MosaicPuzzleScene::createSquares(const entity_t& mosaicBorderLeft, const entity_t& mosaicBorderTop, const entity_t& mosaicBorderRight, const entity_t& mosaicBorderBottom)
{
	for (int i = 0; i < TOTALSQUARES; ++i) { //Creation of the squares and assignation in the different possitions

		squares.push_back(entityManager->addEntity()); //Add the entity to the list
		auto& it = squares.back(); //Reference to the entity

		//Component asignation
		it->getMngr()->addComponent<Transform>(it, positions[indexPositions[i]], Vector2D(0, 0), SQUAREWIDTH, SQUAREWIDTH, 0); //Assign the position
		it->getMngr()->addComponent<Image>(it, &sdlutils().images().at(imgId[i])); //Assign the image
		it->getMngr()->addComponent<RectArea2D>(it, areaLayerManager); //Assign the area
		//Logic Componentes
		it->getMngr()->addComponent<ClickComponent>(it); 
		it->getMngr()->addComponent<TriggerComponent>(it);
		it->getMngr()->addComponent<DragComponent>(it);

		//Add collisions to the square
		auto physicis = it->getMngr()->addComponent<PhysicsBodyComponent>(it);
		physicis->AddObjectToList(mosaicBorderLeft->getMngr()->getComponent<RectArea2D>(mosaicBorderLeft)); //Adds the collision to the MosaicBorder (LEFT)
		physicis->AddObjectToList(mosaicBorderTop->getMngr()->getComponent<RectArea2D>(mosaicBorderTop)); //Adds the collision to the MosaicBorder (TOP)
		physicis->AddObjectToList(mosaicBorderRight->getMngr()->getComponent<RectArea2D>(mosaicBorderRight)); //Adds the collision to the MosaicBorder (RIGHT)
		physicis->AddObjectToList(mosaicBorderBottom->getMngr()->getComponent<RectArea2D>(mosaicBorderBottom)); //Adds the collision to the MosaicBorder (BOTTOM)

		//When tou clicked in the square, keeps the original position of the square

		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this, it]() {
			firstPos = it->getMngr()->getComponent<Transform>(it)->getPos();
			});

		//When the square got released, checks if the puzzle was solved correctly
		it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this,it]() {
			CorrectPositions(it);
			if (Check()) { //If the puzzle is solved correctly, calls Win
				Win();
			}
			});
	}
	//Add collisions between the sqaures
	for (auto actualSquare : squares) {

#ifdef DEBUG
		assert(actualSquare->getMngr()->getComponent<PhysicsBodyComponent>(actualSquare) != nullptr);
#endif // DEBUG

		auto physyicComponent = actualSquare->getMngr()->getComponent<PhysicsBodyComponent>(actualSquare); //Reference to the physics component of the ActualSquare

		for (auto SquareToAdd : squares) { //For each Square, add collision with them
			if (actualSquare != SquareToAdd) {

#ifdef DEBUG
				assert(SquareToAdd->getMngr()->getComponent<PhysicsBodyComponent>(SquareToAdd) != nullptr);
#endif // DEBUG
				physyicComponent->AddObjectToList(SquareToAdd->getMngr()->getComponent<RectArea2D>(SquareToAdd));
			}
		}
	}
}
/// <summary>
/// Create one of the MosaicBorder
/// </summary>
/// <param name="position"></param>
/// <param name="rot"></param>
/// <returns></returns>
entity_t MosaicPuzzleScene::createBorder(const Vector2D& position, float width, float height)
{

	entity_t newBorder = entityManager->addEntity();
	newBorder->getMngr()->addComponent<Transform>(newBorder, position, Vector2D(0, 0), width, height, 0);
	newBorder->getMngr()->addComponent<Image>(newBorder, &sdlutils().images().at("B1"));
	newBorder->getMngr()->addComponent<RectArea2D>(newBorder,areaLayerManager);
	newBorder->getMngr()->addComponent<TriggerComponent>(newBorder);
	return newBorder;

}
MosaicPuzzleScene::MosaicPuzzleScene()
{
	//Assignation of the possitions 

	int index = 0; //Aux index to set the Y position
	for (int i = 0; i < TOTALSQUARES; ++i) { //Initializate the list of positions

		//Resset the index if we change row
		if (i == 3 || i == 6) {
			index = 0;
		}

		//Assign the positions of the different rows
		if(i < 3) positions.push_back(Vector2D(350 ,75 + (index * SQUAREWIDTH))); //First row possitions (0-2)
		else if(i < 6) positions.push_back(Vector2D(350 + SQUAREWIDTH, 75 + (index * SQUAREWIDTH))); //Second row possitions (3-5)
		else positions.push_back(Vector2D(350 + (2 * SQUAREWIDTH), 75 + (index * SQUAREWIDTH))); //Third row possitions (6-8)
		++index;

	}
}

MosaicPuzzleScene::~MosaicPuzzleScene()
{

}

void MosaicPuzzleScene::init(SceneRoomTemplate* sr)
{
	if (!isStarted) {
		isStarted = true;
		room = sr;

#pragma region SpecificEntitiesOfTheScene
		//Background
		//auto background = entityFactory->CreateImageEntity(entityManager,"MosaicBackground",Vector2D(0,0),Vector2D(0,0),32,32,0,ecs::grp::BACKGROUND);

		//Mosaic Border
		auto mosaicBorderLeft = createBorder(Vector2D(330, 90),20,600);
		auto mosaicBorderTop = createBorder(Vector2D(350, 50),600, 20);
		auto mosaicBorderRight = createBorder(Vector2D(950, 90),20,600);
		auto mosaicBorderBottom = createBorder(Vector2D(350, 675),600, 20);
		
		//SquareEntities
		createSquares(mosaicBorderLeft, mosaicBorderTop, mosaicBorderRight, mosaicBorderBottom);
#pragma endregion



#pragma region Buttons
		//Creation of all the buttons
		auto reset = entityFactory->CreateInteractableEntity(entityManager, "clockHorButton", EntityFactory::RECTAREA, Vector2D(800, 0), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		reset->getMngr()->getComponent<ClickComponent>(reset)->connect(ClickComponent::JUST_CLICKED, [this] {
			ResetPuzzle();
		});
		auto resolve = entityFactory->CreateInteractableEntity(entityManager, "clockHorButton", EntityFactory::RECTAREA, Vector2D(400, 0), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		resolve->getMngr()->getComponent<ClickComponent>(resolve)->connect(ClickComponent::JUST_CLICKED, [this] {
			Resolve();
		});
#pragma endregion

		
#pragma region UI

		//BackButton
		auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, []()
			{
				Game::Instance()->getSceneManager()->popScene();
			});

#pragma region Inventory

		//INVENTORY
		//Invntory Background
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1150, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		//InventoryButton
		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
		invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton]() //Lamda function
			{
				//AudioManager::Instance().playSound(buttonSound);
				sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

				// If the inventory is active, activate the items
				if (sr->GetInventory()->getActive()) {
					entityManager->setActive(InventoryBackground, true);

					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
					entityManager->setActive(downButton, true);
					entityManager->setActive(upButton, true);

					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], true);
					}
				}
				else {
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(InventoryBackground, false);
					entityManager->setActive(downButton, false);
					entityManager->setActive(upButton, false);
					inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

					for (int i = 0; i < sr->GetInventory()->getItemNumber(); ++i) {
						invObjects[i]->getMngr()->setActive(invObjects[i], false);
					}
				}
			});

		ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
		UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ upButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(-1);
			});

		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, /*buttonSound,*/ downButton, sr]() {

			//AudioManager::Instance().playSound(buttonSound);
			sr->scrollInventory(1);
			});
		//Log


#pragma endregion

		//Log
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);

		//startDialogue("PuzzleCuervo");

#pragma endregion

	}
	createInvEntities(sr);
}

/// <summary>
/// Corrects the position of the square in case is not in the correct place
/// </summary>
/// <param name="square"></param>
void MosaicPuzzleScene::CorrectPositions(entity_t square)
{
	Transform* actPosition = entityManager->getComponent<Transform>(square);

	//Compare X
	int diff = actPosition->getPos().getX() - firstPos.getX(); //What is the difference between the actual spot to the future spot
	if (diff > 0 && diff >= SQUAREWIDTH / 2) { //if the square is closed to the right than to the left and player wants to go to the right sets the position to the right 
		actPosition->setPosX(firstPos.getX() + SQUAREWIDTH);
	}
	else if (diff < 0 && -diff >= SQUAREWIDTH / 2) { //if the square is closed to the left than to the right and player wants to go to the left sets the position to the left 
		actPosition->setPosX(firstPos.getX() - SQUAREWIDTH);
	}
	else actPosition->setPosX(firstPos.getX());

	//Compare Y
	diff = actPosition->getPos().getY() - firstPos.getY();
	if (diff > 0 && diff >= SQUAREWIDTH / 2) { //if the square is closed to the Top than to the Bottom  and player wants to go to the top sets the position to the top
		actPosition->setPosY(firstPos.getY() + SQUAREWIDTH);
	}
	else if (diff < 0 && -diff >= SQUAREWIDTH / 2) { //if the square is closed to the Bottom than to the Top  and player wants to go to the bottom sets the position to the bottom
		actPosition->setPosY(firstPos.getY() - SQUAREWIDTH);
	}
	else actPosition->setPosY(firstPos.getY());
}

void MosaicPuzzleScene::ResetPuzzle()
{
	for (int i = 0; i < TOTALSQUARES; ++i) {
		squares[i]->getMngr()->getComponent<Transform>(squares[i])->setPos(positions[indexPositions[i]]);
	}
}

void MosaicPuzzleScene::Resolve()
{
	for (int i = 0; i < TOTALSQUARES; ++i) {
		squares[i]->getMngr()->getComponent<Transform>(squares[i])->setPos(positions[i]);
	}
	if (Check()) Win();
}

/// <summary>
/// Checks if the squares are in the correct possitions
/// </summary>
/// <returns></returns> 
bool MosaicPuzzleScene::Check()
{
	int index = 0;
	bool win = true;
	while (win && index < TOTALSQUARES) {
		auto transform = squares[index]->getMngr()->getComponent<Transform>(squares[index])->getPos();
		if (positions[index].getX() != transform.getX() && positions[index].getY() != transform.getY())  { //if the positions aren't the same return false
			win = false;
		}
		++index;
	}
	return win;
}

/// <summary>
/// Sets the solved boolean to true
/// </summary>
void MosaicPuzzleScene::Win()
{
	room->resolvedPuzzle(3);
	setSolved(true);
}

void MosaicPuzzleScene::Exit()
{
}
