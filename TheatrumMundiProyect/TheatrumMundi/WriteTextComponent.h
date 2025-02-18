#pragma once
#include "../../TheatrumMundiProyect/src/ecs/Component.h"
#include <SDL.h>

class WriteTextComponent : public ecs::Component
{
public:
	//ID
	__CMPID_DECL__(ecs::cmp::WRITE_TEXT_COMPONENT);

	WriteTextComponent();

	void update() override;
	void render() override;

	~WriteTextComponent();
};

