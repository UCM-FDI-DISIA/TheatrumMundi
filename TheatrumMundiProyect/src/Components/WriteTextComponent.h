#pragma once

#include <list>
#include <string>

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

template <typename T>
#include "../ecs/Component.h"
#include <SDL.h>
#include "../../TheatrumMundi/TextoInfo.h"

class WriteTextComponent : public ecs::Component
{
private:
	Font& _myFont;
	SDL_Color _color; // White
	//std::list<std::pair<std::string, std::string>> text;
	T textStructure;
public:
	//ID
	__CMPID_DECL__(ecs::cmp::WRITE_TEXT_COMPONENT);

	WriteTextComponent(Font& desiredFont, SDL_Color desiredColor, T text);

	void ShowDialogue(TextInfo dialogue);

	void update() override;
	void render() override;


	~WriteTextComponent();
private:

	TextInfo actualLine = { " ", " " };
	
};

