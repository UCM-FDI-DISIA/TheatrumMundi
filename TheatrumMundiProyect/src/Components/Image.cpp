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
}

Image::Image(Texture* tex, int alpha) :
	_tr(), _tex(tex) {
	_tex->setTextureOpacity(alpha);
}

Image::~Image() {
}

void Image::initComponent() {
	auto mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	_mw = _tr->getWidth();
	_mh = _tr->getHeight();
	assert(_tr != nullptr);
}

void Image::render() {
	Vector2D pos = _tr->getPos() + posoffset;
	SDL_Rect dest = build_sdlrect(pos, _mw,
		_mh);

	assert(_tex != nullptr);
	_tex->render(dest, _tr->getRot());

}

void Image::setAlpha(int alpha) {
	assert(_tex != nullptr);
	_tex->setTextureOpacity(alpha);
}
