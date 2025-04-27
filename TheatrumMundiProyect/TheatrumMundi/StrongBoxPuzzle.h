#pragma once
#include "ScenePuzzleTemplate.h"

class StrongBoxPuzzle: public ScenePuzzleTemplate
{
private:
	std::vector<int> rotSol;
protected:
public:
	StrongBoxPuzzle();
	~StrongBoxPuzzle();
	void init() override;
	void unload() override;
	bool Check() override;
	void Win() override;
	void ResolveScene() override;
};

