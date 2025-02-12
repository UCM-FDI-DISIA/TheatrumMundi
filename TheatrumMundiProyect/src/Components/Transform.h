// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <cassert>

class Transform: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TRANSFORM)

	Transform() :
			_pos(), _vel(), _width(), _height(), _rot() {
	}

	Transform(Vector2D pos, Vector2D vel, float w, float h, float r) :
			_pos(pos), _vel(vel), _width(w), _height(h), _rot(r) {
	}

	virtual ~Transform() {
	}

	void init(Vector2D pos, Vector2D vel, float w, float h, float r) {
		_pos = pos;
		_vel = vel;
		_width = w;
		_height = h;
		_rot = r;
	}

	Vector2D& getPos() {
		return _pos;
	}
	Vector2D& getVel() {
		return _vel;
	}

	float getWidth() {
		return _width;
	}

	void setWidth(float w) {
		_width = w;
	}

	float getHeight() {
		return _height;
	}

	void setHeight(float h) {
		_height = h;
	}

	float getRot() {
		return _rot;
	}

	void setRot(float r) {
		_rot = r;
	}

	void update() override {
		_pos = _pos + _vel;
	}

private:
	Vector2D _pos;
	Vector2D _vel;
	float _width;
	float _height;
	float _rot;
};

