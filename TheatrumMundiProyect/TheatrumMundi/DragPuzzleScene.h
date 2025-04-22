#pragma once
#include "ScenePuzzleTemplate.h"
#include "Vector2D.h"
#include "vector"
class DragPuzzleScene: public ScenePuzzleTemplate 
{
private:
	std::vector<std::vector<Vector2D>> posMat;
	Vector2D _goalPos;
	ecs::entity_t _triggerObj;
	Vector2D auxtiledsize;

protected:



public:
	DragPuzzleScene();

	~DragPuzzleScene();
	
	void init(SceneRoomTemplate* sr) override;

	void unload();

	Vector2D NearMatPoint(Vector2D pos);

	void Exit() override; 

	bool Check() override; 

	void Win() override;
};

