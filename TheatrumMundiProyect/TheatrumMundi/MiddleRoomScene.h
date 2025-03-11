#pragma once
#include "SceneRoomTemplate.h"
class MiddleRoomScene: public SceneRoomTemplate
{ 
private:
	eventToRead _eventToRead;
protected:
	enum MiddleRoomEvent
	{
		LOGENABLE = 0,
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
};

