// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <bitset>
#include <cassert>
#include <vector>

#include "Component.h"
#include "ecs.h"

namespace ecs {

/*
 * A struct that represents a collection of components.
 *
 */

struct Entity {
public:
	Entity(grpId_t gId, EntityManager *mngr) :
			_mngr(mngr), //
			_cmps(), //
			_currCmps(), //
			_active(),  //
			_gId(gId) //
	{
		// We reserve the a space for the maximum number of
		// components. This way we avoid resizing the vector.
		//
		_active = true;
		_alive = true;
		_currCmps.reserve(ecs::maxComponentId);
	}

	// we delete the copy constructor/assignment because it is
	// not clear how to copy the components
	//
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	// Exercise: define move constructor/assignment for class Entity

	// Destroys the entity
	//
	virtual ~Entity() {

		// we delete all available components
		//
		for (auto c : _currCmps)
			delete c;
	}

	// Returns the manager to which this entity belongs
	inline EntityManager* getMngr() {
		return _mngr;
	}

private:

	// We could make the constructors private as well, so only
	// the manager can create instances (because it is a friend)

	friend EntityManager; // so we can update these fields directly from the manager

	EntityManager *_mngr;
	std::array<Component*, maxComponentId> _cmps;
	std::vector<Component*> _currCmps;
	bool _active;
	bool _alive;
	grpId_t _gId;
};

} // end of name space
