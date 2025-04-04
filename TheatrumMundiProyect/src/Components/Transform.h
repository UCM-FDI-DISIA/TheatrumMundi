// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "Game.h"
#include <cassert>

class Transform: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::TRANSFORM)

	Transform() :
			_pos(), _vel(), _width(), _height(), _rot() {
	}

	Transform(Vector2D pos, Vector2D vel, float w, float h, float r) {
		_pos.setX(Game::Instance()->wscreenScale* pos.getX());
		_pos.setY(Game::Instance()->hscreenScale * pos.getY());
		//_pos.setX(pos.getX());
		//_pos.setY(pos.getY());
		_vel = vel;
		_width = (Game::Instance()->wscreenScale * w);
		_height = (Game::Instance()->hscreenScale * h);
		_rot = r;
	}

	virtual ~Transform() {
	}

	void init(Vector2D pos, Vector2D vel, float w, float h, float r) {
		_pos.setX(Game::Instance()->wscreenScale * pos.getX());
		_pos.setY(Game::Instance()->hscreenScale * pos.getY());
		_vel = vel;
		_width = Game::Instance()->wscreenScale * w;
		_height = Game::Instance()->hscreenScale * h;
		_rot = r;
	}

	const Vector2D& getPos() {
		return _pos;
	}
	void addPos(Vector2D vec) {
		int auxX = _pos.getX() + (vec.getX() * Game::Instance()->wscreenScale);
		int auxY = _pos.getY() + (vec.getY() * Game::Instance()->hscreenScale);
		_pos =  Vector2D(auxX,auxY);
	}
	void addPos(int _x=0, int _y =0) {
		_pos.setX(_pos.getX() + (_x* Game::Instance()->wscreenScale));
		_pos.setY(_pos.getY() + (_y * Game::Instance()->hscreenScale));
	}
	void setPosPure(Vector2D vec){
		_pos = vec;
	}

	void setPosX(int _x) {
		_pos.setX(_x * Game::Instance()->wscreenScale);
	}
	void setPosY(int _y) {
		_pos.setY(_y * Game::Instance()->hscreenScale);
	}
	void setPos(Vector2D vec) {
		int auxX =(vec.getX() * Game::Instance()->wscreenScale);
		int auxY = (vec.getY() * Game::Instance()->hscreenScale);
		_pos = Vector2D(auxX, auxY);
	}
	const Vector2D& getVel() {
		return _vel;
	}
	void setVel(Vector2D vec) {
		int auxX = (vec.getX() * Game::Instance()->wscreenScale);
		int auxY = (vec.getY() * Game::Instance()->hscreenScale);
		_vel = Vector2D(auxX, auxY);
	}

	float getWidth() {
		return _width;
	}

	void setWidth(float w) {
		_width = w* Game::Instance()->wscreenScale;
	}

	float getHeight() {
		return _height;
	}

	void setHeight(float h) {
		_height = h* Game::Instance()->hscreenScale;
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

