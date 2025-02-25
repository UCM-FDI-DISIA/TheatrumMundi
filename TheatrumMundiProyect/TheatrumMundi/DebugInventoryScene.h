#pragma once

#include "SceneRoomTemplate.h"


class Inventory;
class DebugInventoryScene : public SceneRoomTemplate
{
private:
	//bool isClickingButton = false;
	//bool act;
protected:
	Inventory* inv2;
public:
	DebugInventoryScene();
	~DebugInventoryScene();
	//void setActive(bool _act);
	void init() override;


};