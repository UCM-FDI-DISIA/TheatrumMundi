#pragma once

#include <string>

#include "../sdlutils/SDLUtils.h"

#include "../ecs/Component.h"

#include "../../TheatrumMundi/TextInfo.h"

template <typename T>
class WriteTextComponent : public ecs::Component
{
private:
	Font& _myFont;
	SDL_Color _color; // White
	T* textStructure; //points to text structure. Log list or TextInfo.
public:
	//ID
	__CMPID_DECL__(ecs::cmp::WRITE_TEXT_COMPONENT);

	WriteTextComponent(Font& desiredFont, SDL_Color desiredColor, T* text);

	void ShowDialogue(T* dialogue);

	void update() override;
	void render() override;


	~WriteTextComponent();
	
};

