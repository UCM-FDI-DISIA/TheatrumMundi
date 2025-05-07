#pragma once
#include "ScenePuzzleTemplate.h"
class Image;

class Box : public ScenePuzzleTemplate
{
public:
	Box();
	~Box();
	void init(SceneRoomTemplate* sr) override;
	void pushButton(int i);
	void Win();
	
private:

	std::vector<Image*> buttonImages;
	std::vector<std::string> buttonImagesIni; 
	int stage = 0;
	bool lastWright = false;
	bool completed = false;

	
};

