#pragma once
#include "SceneTemplate.h"

class ScenePuzzleTemplate: public SceneTemplate
{
	protected:
		bool solved = false;
	public:

	void Exit();
	virtual bool Check() {};
	virtual void Win();
	ScenePuzzleTemplate();
	~ScenePuzzleTemplate();



};