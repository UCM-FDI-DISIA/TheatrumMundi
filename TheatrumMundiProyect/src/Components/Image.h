// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "Vector2D.h"

class Transform;
class Texture;

class Image : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::IMAGE)

		Image();
	Image(Texture* tex, int alpha = 255);
	virtual ~Image();

	void setTexture(Texture* tex) {
		_tex = tex;
	}

	void setAlpha(int alpha);

	void initComponent() override;
	void render() override;
	inline int getW(){ 
		return _mw;
	}
	inline int getH() {
		return _mw;
	}
	inline void setW(int i) {
		_mw = i;
	}
	inline void setH(int i) {
		_mh = i;
	}
	inline Vector2D getPosOffset() {
		return posoffset;
	}
	inline void setPosOffset(int i =0, int j= 0) {
		posoffset.setX(i);
		posoffset.setY(j);
	}
	inline void setPosOffset(Vector2D vec) {
		posoffset = vec;
	}
	inline Texture* GetTexture() { return _tex; };

private:
	Transform* _tr;
	int _mw;
	int _mh;
	Vector2D posoffset;
	Texture* _tex;
};

