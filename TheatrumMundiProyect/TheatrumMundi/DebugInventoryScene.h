#pragma once

#include "SceneRoomTemplate.h"


class Inventory;
class DebugInventoryScene : public SceneRoomTemplate
{
private:
	//bool isClickingButton = false;
protected:
	//Inventory* inv2;
public:
	DebugInventoryScene();
	~DebugInventoryScene();
	void init() override;
	//void render();

};