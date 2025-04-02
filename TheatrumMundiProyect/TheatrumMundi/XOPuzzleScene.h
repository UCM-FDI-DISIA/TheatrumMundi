#pragma once
#include "ScenePuzzleTemplate.h"

class Image;

class XOPuzzleScene : public ScenePuzzleTemplate
{
public:
	XOPuzzleScene();
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

};
