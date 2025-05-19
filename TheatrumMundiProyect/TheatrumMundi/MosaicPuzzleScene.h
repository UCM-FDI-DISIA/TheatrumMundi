#pragma once
#include "ScenePuzzleTemplate.h"
#include <vector>
class Vector2D;
class MosaicPuzzleScene : public ScenePuzzleTemplate
{
private:

	bool squareMoving = false; //if one mosaic piece is moving or not (evitates to move one while other is moving)
	const int TOTALSQUARES = 15; //Total of interactuable squares of the mosaic
	const int SQUAREWIDTH = 205; // Width of each square
	const int SQUAREHEIGHT = 170; // Height of each square
	const int WIDTHCORRECTOR = 270; 
	const int HEIGHTCORRECTOR = 35; 
	std::vector<std::string>imgId = {"Pieza1","Pieza2","Pieza3","Pieza4","Pieza5","Pieza6","Pieza7","Pieza8","Pieza9","Pieza10","Pieza11","Pieza12","Pieza13","Pieza14","Pieza15"}; //array of the images of each square
	std::vector<Vector2D> positions; //Positions of the 
	std::vector<int> indexPositions = {4,5,0,12,2,10,3,1,8,14,7,11,9,6,13}; //Sets the positions of the squares
	Transform* square = nullptr; //Square clicked
	Vector2D freePos; //Saves the free spot Position
	Vector2D originalPos; //Saves the original Pos of the square which is gonna move

	void createSquares();

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

