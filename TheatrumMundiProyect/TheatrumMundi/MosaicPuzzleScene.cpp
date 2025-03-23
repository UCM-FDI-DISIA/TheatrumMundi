#include "MosaicPuzzleScene.h"
#include "PhysicsBodyComponent.h"
#include "SceneRoomTemplate.h"
#include "ClickComponent.h"
#include "RectArea2D.h"
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
		if(i < 3) positions.push_back(Vector2D(100 , 100 + index * SQUAREWIDTH)); //First row possitions (0-2)
		else if(i < 6) positions.push_back(Vector2D(100 + SQUAREWIDTH, 100 + index * SQUAREWIDTH)); //Second row possitions (3-5)
		else positions.push_back(Vector2D(100 + 2 * SQUAREWIDTH, 100 + index * SQUAREWIDTH)); //Third row possitions (6-8)

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
		auto mosaicBorder = entityFactory->CreateInteractableEntity(entityManager, "MosaicBorder", EntityFactory::RECTAREA, Vector2D(0, 0), Vector2D(0, 0), 32, 32, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

		//SquareEntities
		for (int i = 0; i < TOTALSQUARES; ++i) { //Creation of the squares and assignation in the different possitions
			squares.push_back(entityFactory->CreateInteractableEntity(entityManager, imgId[i], EntityFactory::RECTAREA, positions[indexPositions[i]], Vector2D(0, 0), SQUAREWIDTH, SQUAREWIDTH, 0, areaLayerManager, EntityFactory::DRAG, ecs::grp::INTERACTOBJ));

			//Add collisions to the square
			auto physicis = squares.back()->getMngr()->addComponent<PhysicsBodyComponent>(squares.back());
			physicis->AddObjectToList(mosaicBorder->getMngr()->getComponent<RectArea2D>(mosaicBorder)); //Adds the collision to the MosaicBorder

			//When the square got released, checks if the puzzle was solved correctly
			squares.back()->getMngr()->getComponent<ClickComponent>(squares.back())->connect(ClickComponent::JUST_RELEASED, [this /*, sr */ ]() {
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
#pragma endregion

		
	}
//	createInvEntities(sr);
}

/// <summary>
/// Checks if the squares are in the correct possitions
/// </summary>
/// <returns></returns> 
bool MosaicPuzzleScene::Check()
{
	int index = 0;
	bool win = true;
	while (win && index <= TOTALSQUARES) {
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
