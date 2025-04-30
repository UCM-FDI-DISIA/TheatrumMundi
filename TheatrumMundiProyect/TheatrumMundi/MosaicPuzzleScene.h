#pragma once
#include "ScenePuzzleTemplate.h"
#include <vector>
class Vector2D;
class MosaicPuzzleScene : public ScenePuzzleTemplate
{
private:

	bool squareMoving = false; //if one mosaic piece is moving or not (evitates to move one while other is moving)
	const int TOTALSQUARES = 8; //Total of interactuable squares of the mosaic
	const int SQUAREWIDTH = 200; // Size of each square
	std::vector<std::string>imgId = {"B1","B2","B3","B4","B5","B6","B7","B1"}; //array of the images of each square
	std::vector<Vector2D> positions; //Positions of the 
	std::vector<int> indexPositions = {5,3,6,0,2,1,4,7}; //Sets the positions of 
	Transform* square = nullptr; //Square clicked
	Vector2D freePos; //Saves the free spot Position
	Vector2D originalPos; //Saves the original Pos of the square which is gonna move

	void createSquares();
	entity_t createBorder(const Vector2D& position, float Width, float height);

protected:

	std::vector<entity_t> squares; //Array of the squares

public:

	MosaicPuzzleScene();
	~MosaicPuzzleScene();
	void init(SceneRoomTemplate* sr);
	void MoveSquare();
	void ResetPuzzle();
	void Resolve(); //Resolve the puzzle
	bool Check() override;
	void Win() override;

};

