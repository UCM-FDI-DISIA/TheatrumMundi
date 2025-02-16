#pragma once
#include "SceneRoomTemplate.h"
class Room1 : public SceneRoomTemplate
{
private:
protected:
public:
	Room1();
	~Room1();
	void init() override;
	void virtual refresh() override;
	void virtual unload() override;

};

