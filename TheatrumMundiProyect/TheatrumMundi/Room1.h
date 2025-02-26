#pragma once
#include "SceneRoomTemplate.h"
enum Room1Event
{
	initialDialogue = 0,
	event_size,

};
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

