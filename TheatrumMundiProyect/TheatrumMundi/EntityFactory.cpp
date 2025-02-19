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
ecs::entity_t EntityFactory::CreateButton(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot)
{
	ecs::entity_t newButton = entityManager->addEntity();
	entityManager->addComponent<Transform>(newButton, pos, dir, width, height, rot);
	entityManager->addComponent<Image>(newButton, &sdlutils().images().at(idImage));
	entityManager->addComponent<RectArea2D>(newButton);
	entityManager->addComponent<ClickComponent>(newButton);
	entityManager->addComponent<TriggerComponent>(newButton);
	return newButton;
}

//CREAMOS BOTON DE SCROLL
ecs::entity_t EntityFactory::CreateScrollButton(ecs::EntityManager* entityManager, std::string idImage,
	Vector2D pos, Vector2D dir, int width, int height, int rot,
	Vector2D _dirScroll,float _time)
{
	ecs::entity_t newScrollButton = entityManager->addEntity();
	entityManager->addComponent<Transform>(newScrollButton, pos, dir, width, height, rot);
	entityManager->addComponent<Image>(newScrollButton, &sdlutils().images().at(idImage));
	entityManager->addComponent<RectArea2D>(newScrollButton);
	entityManager->addComponent<ScrollComponent>(newScrollButton, _dirScroll, _time);
	entityManager->addComponent<ClickComponent>(newScrollButton);
	entityManager->addComponent<TriggerComponent>(newScrollButton);
	return newScrollButton;
}

