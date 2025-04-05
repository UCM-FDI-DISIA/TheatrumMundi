#pragma once
#include "ScenePuzzleTemplate.h"
#include <vector>
class Vector2D;
class MosaicPuzzleScene : public ScenePuzzleTemplate
{
private:

	const int TOTALSQUARES = 8; //Total of interactuable squares of the mosaic
	const int SQUAREWIDTH = 200; // Size of each square
	std::vector<std::string>imgId = {"B1","B2","B3","B4","B5","B6","B7","B1"}; //array of the images of each square
	std::vector<Vector2D> positions; //Positions of the 
	std::vector<int> indexPositions = {5,3,6,0,2,1,4,7}; //Sets the positions of 
	Vector2D firstPos; //First Position of the square

	void createSquares(const entity_t& mosaicBorderLeft, const entity_t& mosaicBorderTop, const entity_t& mosaicBorderRight, const entity_t& mosaicBorderBottom);
	entity_t createBorder(const Vector2D& position, float Width, float height);
protected:
	std::vector<entity_t> squares; //Array of the squares
public:

	MosaicPuzzleScene();
	~MosaicPuzzleScene();
	void init(SceneRoomTemplate* sr);
	void CorrectPositions(entity_t square);
	void ResetPuzzle();
	bool Check() override;
	void Win() override;
	void Exit() override;

};

