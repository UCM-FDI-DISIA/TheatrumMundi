#pragma once
#include "../../TheatrumMundiProyect/src/ecs/Component.h"

#include <list>
#include <string>

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

template <typename T>

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

	void update() override;
	void render() override;

	~WriteTextComponent();
};

