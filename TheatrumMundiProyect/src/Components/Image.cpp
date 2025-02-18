// This file is part of the course TPV2@UCM - Samir Genaim

#include "Image.h"

#include <cassert>

#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"

#include "../sdlutils/SDLUtils.h"

Image::Image() :
	_tr(), _tex() {
	_show = true;
}

Image::Image(Texture* tex) :
	_tr(), _tex(tex) {
	_show = true;
}

Image::~Image() {
}

void Image::initComponent() {
	auto mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void Image::render() {
	if (_show)
	{
		SDL_Rect dest = build_sdlrect(_tr->getPos(), _tr->getWidth(),
			_tr->getHeight());

		assert(_tex != nullptr);
		_tex->render(dest, _tr->getRot());

		/*
		Font& myFont = sdlutils().fonts().at("ARIAL24");
		SDL_Color color = { 255, 255, 255, 255 }; // White
		std::string text = "author1";

		Texture* textTexture = new Texture(sdlutils().renderer(), text, myFont, color); //convert text to texture
		SDL_Rect dstRect = { 500, 500, textTexture->width(), textTexture->height()}; //destiny rect
		textTexture->render(dstRect, 0.0); //render
		*/

	}


}
