#pragma once
#include "SceneTemplate.h"

class ScenePuzzleTemplate: public SceneTemplate
{
	protected:
		bool solved = false;
		SceneRoomTemplate* room;
		bool logActive = false;
	public:

	virtual void Exit();
	virtual bool Check() { return true; };
	virtual void Win() {};
	ScenePuzzleTemplate(SceneRoomTemplate *r);
	~ScenePuzzleTemplate();



};