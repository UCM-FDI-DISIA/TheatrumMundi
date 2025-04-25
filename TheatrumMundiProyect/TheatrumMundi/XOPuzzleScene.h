#pragma once
#include "ScenePuzzleTemplate.h"
#include "TombPuzzleScene.h"
class Image;

class XOPuzzleScene : public ScenePuzzleTemplate
{
public:
	XOPuzzleScene(TombPuzzleScene* tomb);
	~XOPuzzleScene();
	void init(SceneRoomTemplate* sr) override;

	void pushButton(int i);
	void resetButtons();
	void updateButtonImage(int index);
	bool checkWin();

	void Win();



private:

	std::vector<bool> XO; //X = false, O = true	
	std::vector<Image*> buttonImages;
	TombPuzzleScene* _tomb;
};
