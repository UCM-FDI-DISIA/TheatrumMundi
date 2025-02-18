#pragma once
#include "../ecs/Component.h"
#include <SDL.h>
#include "../../TheatrumMundi/TextoInfo.h"

class WriteTextComponent : public ecs::Component
{
public:
	//ID
	__CMPID_DECL__(ecs::cmp::WRITE_TEXT_COMPONENT);

	WriteTextComponent();

	void ShowDialogue(TextInfo dialogue);

	void update() override;
	void render() override;


	~WriteTextComponent();
private:

	TextInfo actualLine = { " ", " " };
	
};

