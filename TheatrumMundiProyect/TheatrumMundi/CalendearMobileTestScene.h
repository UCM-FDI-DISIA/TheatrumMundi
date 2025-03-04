#pragma once
#include "SceneRoomTemplate.h"
class CalendearMobileTestScene : public SceneRoomTemplate
{
public:

	CalendearMobileTestScene();
	~CalendearMobileTestScene();
	void init() override;
	void virtual refresh() override;
	void virtual unload() override;

};

