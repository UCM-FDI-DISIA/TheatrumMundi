#pragma once

#include "../src/ecs/Entity.h"
#include "../src/ecs/ecs.h"

class Vector2D;

class EntityFactory {
public:

	EntityFactory();
	~EntityFactory();

	enum AreaType { RECTAREA, CIRCLEAREA }; //DECIDE IF THE AREA IS RECT OR CIRCLE
	enum Dragging { DRAG, NODRAG }; //DECIDE IF THE ELEMENT IS DRAGGED OR NOT

	//Creates an Image Entity (BACKGROUND, VISUAL ITEM FROM INVENTORY, ETC)
	ecs::entity_t CreateImageEntity(ecs::EntityManager* _entityManager,const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot);

	//Creates an interactable entity (BUTTON) or drag interactable entity (OBJECT TO MOVE IN PUZZLES)
	ecs::entity_t CreateInteractableEntity(ecs::EntityManager* _entityManager,const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot,
	Dragging _drag); //DRAG ENTITY

	//Creates a scroll interactable entity (MOVES FROM ONE ROOM TO ANOTHER) or scroll dragging interactable entity (SCROLL BUTTON TO LOG OR INVENTORY)
	ecs::entity_t CreateInteractableEntityScroll(ecs::EntityManager* _entityManager,const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot,
	Vector2D _dirScroll, float _time,
	Dragging _drag); //DRAG SCROLL ENTITY
};