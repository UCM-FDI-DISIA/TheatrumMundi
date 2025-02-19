#pragma once

#include "../src/ecs/Entity.h"
#include "../src/utils/Singleton.h"

#include "../src/ecs/ecs.h"

class Vector2D;

class EntityFactory {
public:
	EntityFactory();
	~EntityFactory();

	//Creates a common button
	ecs::entity_t CreateButton(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot);

	//Creates a scroll button
	ecs::entity_t CreateScrollButton(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot,
	Vector2D _dirScroll, float _time);

private:

};