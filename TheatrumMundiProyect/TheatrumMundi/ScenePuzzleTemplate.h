#pragma once
#include "SceneTemplate.h"

class ScenePuzzleTemplate: public SceneTemplate
{
	protected:
		bool solved = false;
		SceneRoomTemplate* room;
		bool logActive = false;
		Vector2D originalPos;
	public:
	inline void setOriginalPos(Vector2D pos) { originalPos = pos; };
	inline Vector2D getOriginalPos() { return originalPos; };
	std::vector<ecs::Entity*> invObjects;
	virtual void Exit();
	virtual bool Check() { return true; };
	virtual void Win() {};
	ScenePuzzleTemplate();
	~ScenePuzzleTemplate();



};