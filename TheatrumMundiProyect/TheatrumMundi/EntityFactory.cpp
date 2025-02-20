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

//CREAMOS BOTON COMÚN
ecs::entity_t EntityFactory::CreateInteractableEntity(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot, bool _dragging)
{
	ecs::entity_t newElement = entityManager->addEntity();
	entityManager->addComponent<Transform>(newElement, pos, dir, width, height, rot);
	entityManager->addComponent<Image>(newElement, &sdlutils().images().at(idImage));
	entityManager->addComponent<RectArea2D>(newElement);
	entityManager->addComponent<ClickComponent>(newElement);
	entityManager->addComponent<TriggerComponent>(newElement);
	if (_dragging) entityManager->addComponent<DragComponent>(newElement);
	return newElement;
}

//CREAMOS BOTON DE SCROLL
ecs::entity_t EntityFactory::CreateInteractableEntityScroll(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot,
	Vector2D _dirScroll,float _time, bool _dragging)
{
	ecs::entity_t newElement = entityManager->addEntity();
	entityManager->addComponent<Transform>(newElement, pos, dir, width, height, rot);
	entityManager->addComponent<Image>(newElement, &sdlutils().images().at(idImage));
	entityManager->addComponent<RectArea2D>(newElement);
	entityManager->addComponent<ScrollComponent>(newElement, _dirScroll, _time);
	entityManager->addComponent<ClickComponent>(newElement);
	entityManager->addComponent<TriggerComponent>(newElement);
	if (_dragging) entityManager->addComponent<DragComponent>(newElement);
	return newElement;
}

