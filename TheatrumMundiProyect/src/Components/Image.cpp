// This file is part of the course TPV2@UCM - Samir Genaim

#include "Image.h"

#include <cassert>

#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"

Image::Image() :
		_tr(), _tex() {
}

Image::Image(Texture *tex) :
		_tr(), _tex(tex) {
}

Image::~Image() {
}

void Image::initComponent() {
	auto mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}

void Image::render() {

	SDL_Rect dest = build_sdlrect(_tr->getPos(), _tr->getWidth(),
			_tr->getHeight());

	assert(_tex != nullptr);
	_tex->render(dest, _tr->getRot());

}
