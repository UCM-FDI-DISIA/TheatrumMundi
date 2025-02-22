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
	bool Check() override;

private:
	std::array<int, 10> comb1;
	std::array<int, 10> comb2;
	std::array<int, 10> comb3;

	int num1;
	int num2;
	int num3;
};

