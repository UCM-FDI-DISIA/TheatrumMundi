#pragma once
#include "SceneTemplate.h"
class InitialScene : public SceneTemplate
{
private:

public:
	InitialScene();
	~InitialScene();
	void init() override;
	void refresh()override;
};

