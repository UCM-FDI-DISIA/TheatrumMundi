#pragma once
#include "SceneRoomTemplate.h"
class MiddleRoomLogDebug : public SceneRoomTemplate
{
private:
	eventToRead _eventToRead;
protected:
	/*
	* The number in BAD and GOOD represents the alive characters in scene
	*/
	enum MiddleRoomEvent
	{
		FIRST_DIALOGUE = 0,
		//end of room1
		AFTER_ROOM1_GOOD3,
		AFTER_ROOM1_BAD2,
		//ends of room2
		AFTER_ROOM2_GOOD2,
		AFTER_ROOM2_GOOD3,
		AFTER_ROOM2_BAD1,
		AFTER_ROOM2_BAD2,
		//end of room3
		AFTER_ROOM3_GOOD3,
		AFTER_ROOM3_GOOD2,
		AFTER_ROOM3_BAD1,
		AFTER_ROOM3_BAD2,
		//misc
		LOGENABLE,
		LOGDESABLE,
		MIDDLEROOMEVENTSIZE
	};
public:
	MiddleRoomLogDebug();
	~MiddleRoomLogDebug();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;
	void endDialogue() override;
};

