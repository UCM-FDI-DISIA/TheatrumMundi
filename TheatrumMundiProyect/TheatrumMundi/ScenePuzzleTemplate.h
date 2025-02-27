#pragma once
#include "SceneTemplate.h"

class ScenePuzzleTemplate: public SceneTemplate
{
	protected:
		bool solved = false;
	public:

	virtual void Exit();
	virtual bool Check() { return true; };
	virtual void Win() {};
	ScenePuzzleTemplate();
	~ScenePuzzleTemplate();



};