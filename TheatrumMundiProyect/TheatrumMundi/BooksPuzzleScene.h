#pragma once
#include "ScenePuzzleTemplate.h"

class array;

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
	std::array<int, 10> comb;

	int num1;
	int num2;
	int num3;
};

