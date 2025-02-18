// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"

class Transform;
class Texture;

class Image : public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::IMAGE)

	Image();
	Image(Texture* tex);
	virtual ~Image();

	void setTexture(Texture* tex) {
		_tex = tex;
	}

	void initComponent() override;
	void render() override;

	bool getShow()
	{
		return _show;
	}

	void setShow(bool show)
	{
		_show = show;
	}

private:
	Transform* _tr;
	Texture* _tex;
	bool _show;
};

