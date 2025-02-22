#include "EntityFactory.h"

#include "../src/ecs/Manager.h"

#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

EntityFactory::EntityFactory(){}

EntityFactory::~EntityFactory(){}

//CREATES IMAGE ENTITY
ecs::entity_t EntityFactory::CreateImageEntity(ecs::EntityManager* _entityManager, std::string _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot)
{
	ecs::entity_t newElement = _entityManager->addEntity();
	_entityManager->addComponent<Transform>(newElement, _pos, _dir, _width, _height, _rot);
	_entityManager->addComponent<Image>(newElement, &sdlutils().images().at(_idImage));
	if (_typeRect == RECTAREA)_entityManager->addComponent<RectArea2D>(newElement);
	else if (_typeRect == CIRCLEAREA) _entityManager->addComponent<CircleArea2D>(newElement)->setLocalPos(Vector2D(_width/2,_height/2));
	return newElement;
}

//CREATES COMMON BUTTON (WITH DRAG IS AN OBJECT TO MOVE IN PUZZLES)
ecs::entity_t EntityFactory::CreateInteractableEntity(ecs::EntityManager* _entityManager, std::string _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot, 
	Dragging _drag)
{
	ecs::entity_t newElement = _entityManager->addEntity();
	_entityManager->addComponent<Transform>(newElement, _pos, _dir, _width, _height, _rot);
	_entityManager->addComponent<Image>(newElement, &sdlutils().images().at(_idImage));
	if (_typeRect == RECTAREA)_entityManager->addComponent<RectArea2D>(newElement);
	else if (_typeRect == CIRCLEAREA) _entityManager->addComponent<CircleArea2D>(newElement)->setLocalPos(Vector2D(_width/2,_height/2));
	_entityManager->addComponent<ClickComponent>(newElement);
	_entityManager->addComponent<TriggerComponent>(newElement);
	if (_drag == DRAG) _entityManager->addComponent<DragComponent>(newElement);
	return newElement;
}

//CREATES SCROLL BUTTON (WITH DRAG IS AN SCROLLBAR TO LOG OR INVENTORY)
ecs::entity_t EntityFactory::CreateInteractableEntityScroll(ecs::EntityManager* _entityManager, std::string _idImage, AreaType _typeRect,
	Vector2D _pos, Vector2D _dir, int _width, int _height, int _rot,
	Vector2D _dirScroll,float _time, 
	Dragging _drag)
{
	ecs::entity_t newElement = _entityManager->addEntity();
	_entityManager->addComponent<Transform>(newElement, _pos, _dir, _width, _height, _rot);
	_entityManager->addComponent<Image>(newElement, &sdlutils().images().at(_idImage));
	if (_typeRect == RECTAREA)_entityManager->addComponent<RectArea2D>(newElement);
	else if (_typeRect == CIRCLEAREA) _entityManager->addComponent<CircleArea2D>(newElement)->setLocalPos(Vector2D(_width/2,_height/2));
	_entityManager->addComponent<ScrollComponent>(newElement, _dirScroll, _time);
	_entityManager->addComponent<ClickComponent>(newElement);
	_entityManager->addComponent<TriggerComponent>(newElement);
	if (_drag == DRAG) _entityManager->addComponent<DragComponent>(newElement);
	return newElement;
}

