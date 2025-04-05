#pragma once

#include "../src/ecs/Entity.h"
#include "../src/ecs/ecs.h"

class Vector2D;
class Area2DLayerManager;

class EntityFactory {
public:

	EntityFactory(ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager);
	~EntityFactory();

	enum AreaType { RECTAREA, CIRCLEAREA }; //DECIDE IF THE AREA IS RECT OR CIRCLE
	enum Dragging { DRAG, NODRAG }; //DECIDE IF THE ELEMENT IS DRAGGED OR NOT
	enum ScrollType { SCROLLNORMAL, SCROLLINVERSE };

	//Creates an Image Entity (BACKGROUND, VISUAL ITEM FROM INVENTORY, ETC)
	ecs::entity_t CreateImageEntity(ecs::EntityManager* _entityManager,const std::string& _idImage,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot,
	ecs::grpId_t gId); //GROUP OF ENTITIES

	//Creates an interactable entity (BUTTON) or drag interactable entity (OBJECT TO MOVE IN PUZZLES)
	ecs::entity_t CreateInteractableEntity(ecs::EntityManager* _entityManager,const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot, Area2DLayerManager* _myLayer,
	Dragging _drag, //DRAG ENTITY
	ecs::grpId_t gId); //GROUP OF ENTITIES

	ecs::entity_t CreateInteractableEntityNotMoveSprite(ecs::EntityManager* _entityManager, const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot, Area2DLayerManager* _myLayer,
	Dragging _drag,
	ecs::grpId_t gId);

	//Create a Image with rectCollider.
	ecs::entity_t CreateRectImageWithCollider(ecs::EntityManager* _entityManager, const std::string& _idImage,
		Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot, Area2DLayerManager* _myLayer,
		ecs::grpId_t gId);

	//Creates a scroll interactable entity (MOVES FROM ONE ROOM TO ANOTHER) or scroll dragging interactable entity (SCROLL BUTTON TO LOG OR INVENTORY)
	ecs::entity_t CreateInteractableEntityScroll(ecs::EntityManager* _entityManager,const std::string& _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot, Area2DLayerManager* _myLayer, 
	int _velocityScroll, float _time, ScrollType _isInverted, int _numPhasesScrolling,
	Dragging _drag,	//DRAG SCROLL ENTITY
	ecs::grpId_t gId); //GROUP OF ENTITIES
	//Create a drag obj with a tiled collider
	ecs::entity_t CreateInteractableEntityTiledCollider(ecs::EntityManager* _entityManager, const std::string& _idImage,
		Vector2D _pos, Vector2D _dir, int _width, int _height, int col, int fil, int _rot, Area2DLayerManager* _myLayer, ecs::grpId_t _gId);
	ecs::entity_t CreateTriggerEntity(ecs::EntityManager* _entityManager, const std::string& _idImage,
		Vector2D _pos, Vector2D _dir, int _width, int _height , int _rot, Area2DLayerManager* _myLayer, ecs::grpId_t _gId);

private:

	ecs::EntityManager* _myEntityManager;
	Area2DLayerManager* _myAreaLayerManager;
};