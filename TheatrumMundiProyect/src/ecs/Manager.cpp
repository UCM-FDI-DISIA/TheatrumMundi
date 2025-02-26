// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"

namespace ecs {

EntityManager::EntityManager() :
		_hdlrs(), //
		_entsByGroup() //
{

	// for each group we reserve space for 100 entities,
	// just to avoid copies
	//
	for (auto &groupEntities : _entsByGroup) {
		groupEntities.reserve(100);
	}
}

EntityManager::~EntityManager() {

	// delete all entities
	//
	for (auto &ents : _entsByGroup) {
		for (auto e : ents)
			delete e;
	}
}

void EntityManager::setActiveGroup(grpId_t gId , bool active){
	auto& ent = getEntities(gId);
	for (auto a : ent) a->_active = active;

}

void EntityManager::refresh() {

	// remove dead entities from the groups lists, and also those
	// do not belong to the group anymore
	for (ecs::grpId_t gId = 0; gId < ecs::maxGroupId; gId++) {
		auto &groupEntities = _entsByGroup[gId];
		groupEntities.erase(
				std::remove_if(groupEntities.begin(), groupEntities.end(),
						[this](Entity *e) {
							if (!isActive(e)) {
								return false;
							} else {
								delete e;
								return true;
							}
						}), groupEntities.end());
	}

}

} // end of namespace
