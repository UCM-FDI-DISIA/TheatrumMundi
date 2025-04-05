#pragma once
#include "SceneTemplate.h"
#include "ecs.h"
class InitialScene : public SceneTemplate
{
private:
	ecs::entity_t _loadimg;

public:
	InitialScene();
	~InitialScene();
	void init() override;
};

