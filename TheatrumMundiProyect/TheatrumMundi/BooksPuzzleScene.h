#pragma once
#include "ScenePuzzleTemplate.h"

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
	int _actualHour;
	int _actualMinute;
};

