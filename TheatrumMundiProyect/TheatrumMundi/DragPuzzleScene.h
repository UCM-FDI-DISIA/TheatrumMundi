#pragma once
#include "ScenePuzzleTemplate.h"
#include "Vector2D.h"
class DragPuzzleScene: public ScenePuzzleTemplate 
{
private:
	Vector2D posMat[7][6];
	Vector2D _goalPos;
	Transform* _triggerObj;

protected:



public:
	DragPuzzleScene();

	~DragPuzzleScene();

	void init(SceneRoomTemplate* sr) override;

	void unload();

	void Exit() override; 

	bool Check() override; 

	void Win() override;
};

