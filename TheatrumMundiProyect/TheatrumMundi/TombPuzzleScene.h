#pragma once
#include "ScenePuzzleTemplate.h"
class TombPuzzleScene : public ScenePuzzleTemplate
{
private:
	bool XOpuzzleResolve;
	bool dragPuzzleResolve;
	bool win = false;
	bool open = false;
	bool startDg = false;
	int numDialog = 0;
	ecs::entity_t TombBackGround;
	ecs::entity_t XOAccess;
	ecs::entity_t dragAccess;
	ecs::entity_t neckclace;
	ecs::entity_t shovel;
public:
	TombPuzzleScene();
	~TombPuzzleScene();
	void init(SceneRoomTemplate* sr) override;
	bool Check() override;
	void Win() override;
	void refresh() override;

	inline void setXOpuzzle(bool isResolved) { XOpuzzleResolve = isResolved;
	startDg = true;
	numDialog++;
	};
	inline void setDragpuzzle(bool isResolved) { dragPuzzleResolve = isResolved;
	startDg = true;
	numDialog++;
	};
};

