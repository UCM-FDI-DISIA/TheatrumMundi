#pragma once
#include "ScenePuzzleTemplate.h"

class array;
class vector;

class BooksPuzzleScene : public ScenePuzzleTemplate
{
public:
	BooksPuzzleScene();
	~BooksPuzzleScene();
	void init() override;
	void refresh() override;
	void unload() override;
	bool Check() override; //HERE WE PUT THE CORRECT COMBINATION

private:
	//std::array<int, 10> comb;
	//std::array<int, 3> myComb;

	std::vector<int> comb;
	std::vector<int> myComb;
};

