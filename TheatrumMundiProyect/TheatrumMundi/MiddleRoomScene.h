#pragma once
#include "SceneRoomTemplate.h"
class MiddleRoomScene: public SceneRoomTemplate
{ 
private:
	eventToRead _eventToRead;
protected:
	/*
	* The number in BAD and GOOD represents the alive characters in scene
	*/
	enum MiddleRoomEvent
	{
		FIRST_DIALOGUE= 0,
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
		AFTER_ROOM3_GOOD2SL,
		AFTER_ROOM3_GOOD2KL,
		AFTER_ROOM3_BAD2SK,
		AFTER_ROOM3_BAD1K,
		AFTER_ROOM3_BAD1S,
		
		//misc
		LOGENABLE,
		LOGDESABLE,
		MIDDLEROOMEVENTSIZE
	};
public:
	MiddleRoomScene();
	~MiddleRoomScene();
	void init() override;
	void resolvedPuzzle(int i) override;
	void refresh() override;
	void unload() override;
	void endDialogue() override;
};

