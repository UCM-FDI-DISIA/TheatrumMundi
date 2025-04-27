#pragma once
#include "ScenePuzzleTemplate.h"

class StrongBoxPuzzle: public ScenePuzzleTemplate
{
private:
	std::vector<int> rotSol;
	std::vector<Transform*> wheelstr;
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

