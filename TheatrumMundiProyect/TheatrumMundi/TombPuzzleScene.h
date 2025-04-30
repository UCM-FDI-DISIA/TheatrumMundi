#pragma once
#include "ScenePuzzleTemplate.h"
class TombPuzzleScene : public ScenePuzzleTemplate
{
private:
	bool XOpuzzleResolve;
	bool dragPuzzleResolve;
public:
	TombPuzzleScene();
	~TombPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool Check() override;
	void Win() override;

	inline void setXOpuzzle(bool isResolved) { XOpuzzleResolve = isResolved; };
	inline void setDragpuzzle(bool isResolved) { dragPuzzleResolve = isResolved; };
};

