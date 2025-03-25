#include "MosaicPuzzleScene.h"
#include "PhysicsBodyComponent.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "RectArea2D.h"
#include "ClickableSpriteComponent.h"
#include "SDLUtils.h"
#include "Image.h"
#include "TriggerComponent.h"
#include "DragComponent.h"
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
		if(i < 3) positions.push_back(Vector2D(300 , 100 + (index * SQUAREWIDTH))); //First row possitions (0-2)
		else if(i < 6) positions.push_back(Vector2D(300 + SQUAREWIDTH, 100 + (index * SQUAREWIDTH))); //Second row possitions (3-5)
		else positions.push_back(Vector2D(300 + (2 * SQUAREWIDTH), 100 + (index * SQUAREWIDTH))); //Third row possitions (6-8)
		++index;

	}
}

MosaicPuzzleScene::~MosaicPuzzleScene()
{
}

void MosaicPuzzleScene::init(/*SceneRoomTemplate* sr*/)
{
	if (!isStarted) {
		isStarted = true;
	//	room = sr;

#pragma region SpecificEntitiesOfTheScene
		//Background
		//auto background = entityFactory->CreateImageEntity(entityManager,"MosaicBackground",Vector2D(0,0),Vector2D(0,0),32,32,0,ecs::grp::BACKGROUND);

		//Mosaic Border
		//auto mosaicBorderLeft = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, Vector2D(100, 100), Vector2D(0, 0), 50, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		//auto mosaicBorderTop = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, Vector2D(580, -500), Vector2D(0, 0), 50, 500, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		//auto mosaicBorderRight = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, Vector2D(900, 100), Vector2D(0, 0), 50, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		//auto mosaicBorderBottom = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, Vector2D(580, 850), Vector2D(0, 0), 50, 500, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		
		//SquareEntities
		for (int i = 0; i < TOTALSQUARES; ++i) { //Creation of the squares and assignation in the different possitions
			squares.push_back(entityManager->addEntity());
			auto& it = squares.back();

			//Asignación de los componentes
			it->getMngr()->addComponent<Transform>(it,positions[indexPositions[i]],Vector2D(0,0),SQUAREWIDTH,SQUAREWIDTH,0);
			it->getMngr()->addComponent<Image>(it, &sdlutils().images().at(imgId[i]));
			it->getMngr()->addComponent<RectArea2D>(it,areaLayerManager);
			it->getMngr()->addComponent<ClickComponent>(it);
			it->getMngr()->addComponent<TriggerComponent>(it);
			it->getMngr()->addComponent<DragComponent>(it);

			//Add collisions to the square
			auto physicis = it->getMngr()->addComponent<PhysicsBodyComponent>(it);
		/*	physicis->AddObjectToList(mosaicBorderLeft->getMngr()->getComponent<RectArea2D>(mosaicBorderLeft));
			physicis->AddObjectToList(mosaicBorderTop->getMngr()->getComponent<RectArea2D>(mosaicBorderTop)); //Adds the collision to the MosaicBorder
			physicis->AddObjectToList(mosaicBorderRight->getMngr()->getComponent<RectArea2D>(mosaicBorderRight)); //Adds the collision to the MosaicBorder
			physicis->AddObjectToList(mosaicBorderBottom->getMngr()->getComponent<RectArea2D>(mosaicBorderBottom)); //Adds the collision to the MosaicBorder*/

			//When tou clicked in the square, keeps the original position of the square

			it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_CLICKED, [this /*, sr */,it]() {
				firstPos = it->getMngr()->getComponent<Transform>(it)->getPos();
			});

			//When the square got released, checks if the puzzle was solved correctly
			 it->getMngr()->getComponent<ClickComponent>(it)->connect(ClickComponent::JUST_RELEASED, [this /*, sr */,it ]() {
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
#pragma endregion



#pragma region Buttons
		//Creation of all the buttons
		auto reset = entityFactory->CreateInteractableEntity(entityManager, "clockHorButton", EntityFactory::RECTAREA, Vector2D(800, 0), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		reset->getMngr()->getComponent<ClickComponent>(reset)->connect(ClickComponent::JUST_CLICKED, [this] {
			ResetPuzzle();
			});
#pragma endregion

		
	}
//	createInvEntities(sr);
}

/// <summary>
/// Corrects the position of the square in case is not in the correct place
/// </summary>
/// <param name="square"></param>
void MosaicPuzzleScene::CorrectPositions(entity_t square)
{
	auto& actPosition = entityManager->getComponent<Transform>(square)->getPos();

	//Compare X
	int diff = actPosition.getX() - firstPos.getX(); //What is the difference between the actual spot to the future spot
	if (diff > 0 && diff >= SQUAREWIDTH / 2) { //if the square is closed to the right than to the left and player wants to go to the right sets the position to the right 
		actPosition.setX(firstPos.getX() + SQUAREWIDTH);
	}
	else if (diff < 0 && -diff >= SQUAREWIDTH / 2) { //if the square is closed to the left than to the right and player wants to go to the left sets the position to the left 
		actPosition.setX(firstPos.getX() - SQUAREWIDTH);
	}
	else actPosition.setX(firstPos.getX());

	//Compare Y
	diff = actPosition.getY() - firstPos.getY();
	if (diff > 0 && diff >= SQUAREWIDTH / 2) { //if the square is closed to the Top than to the Bottom  and player wants to go to the top sets the position to the top
		actPosition.setY(firstPos.getY() + SQUAREWIDTH);
	}
	else if (diff < 0 && -diff >= SQUAREWIDTH / 2) { //if the square is closed to the Bottom than to the Top  and player wants to go to the bottom sets the position to the bottom
		actPosition.setY(firstPos.getY() - SQUAREWIDTH);
	}
	else actPosition.setY(firstPos.getY());
}

void MosaicPuzzleScene::ResetPuzzle()
{
	for (int i = 0; i < TOTALSQUARES; ++i) {
		squares[i]->getMngr()->getComponent<Transform>(squares[i])->getPos().set(positions[indexPositions[i]]);
	}
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
	room->resolvedPuzzle(2);
	setSolved(true);
}

void MosaicPuzzleScene::Exit()
{
}
