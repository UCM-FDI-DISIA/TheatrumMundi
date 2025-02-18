#pragma once

#include "SceneRoomTemplate.h"
class DialogTestScene: public SceneRoomTemplate
{
private:
protected:
public:
	DialogTestScene();
	~DialogTestScene();
	void init() override;
	void virtual refresh() override;
	void virtual unload() override;



};

