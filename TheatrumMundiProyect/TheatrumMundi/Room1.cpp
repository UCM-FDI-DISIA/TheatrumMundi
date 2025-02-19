#include "Room1.h"
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"


Room1::Room1(): SceneRoomTemplate()
{
}

Room1::~Room1()
{
}

void Room1::init()
{
	if (!isStarted) {
		//auto _fighter = entityManager->addEntity();
		//auto _fighterTransform = entityManager->addComponent<Transform>(_fighter, Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0);
		//entityManager->addComponent<Image>(_fighter, &sdlutils().images().at("prueba"));
		//
		//entityManager->addComponent<CircleArea2D>(_fighter)->setLocalPos(Vector2D(250,250));
		//
		//
		////ClickComponent* clk = entityManager->addComponent<ClickComponent>(_fighter);
		////clk->connect(ClickComponent::JUST_CLICKED, []() { std::cout << "CLICKED\n"; });
		//
		//TriggerComponent* trg = entityManager->addComponent<TriggerComponent>(_fighter);
		///*trg->connect(TriggerComponent::CURSOR_ENTERED, []() { std::cout << "ENTERED\n";  });
		//trg->connect(TriggerComponent::CURSOR_LEFT, []() { std::cout << "LEFT\n";  });*/
		//trg->connect(TriggerComponent::AREA_ENTERED, [trg]() {
		//	std::cout << "AREA2D IN\n";  
		//	std::list<ecs::entity_t> ents = trg->triggerContextEntities();
		//	for (ecs::entity_t e : ents) {
		//		if (e->getMngr()->getComponent<ClickComponent>(e) != nullptr)
		//			std::cout << "Has click\n";
		//	}
		//});
		//trg->connect(TriggerComponent::AREA_LEFT, []() { std::cout << "AREA2D OUT\n";  });
		//
		//DragComponent* drg = entityManager->addComponent<DragComponent>(_fighter);
		//drg->connect(DragComponent::DRAG, []() { std::cout << "DRAGGING\n"; });
		//
		////BOTON
		//auto _button = entityManager->addEntity();
		//auto _buttonTransform = entityManager->addComponent<Transform>(_button, Vector2D(500, 500), Vector2D(0, 0), 500, 500, 0);
		//entityManager->addComponent<Image>(_button, &sdlutils().images().at("prueba"));
		//
		//entityManager->addComponent<RectArea2D>(_button);
		//
		////ScrollComponent
		////Velocity only X or Y (needs to be positive), time scrolling
		//ScrollComponent* _buttonScroll = entityManager->addComponent<ScrollComponent>(_button,Vector2D(0,5),100.0f);
		////Add element to scroll, needs to be a transform
		//
		////ScrollComponent* _buttonScrolling = entityManager->getComponent
		////HACEMOS GETTER AQUI PARA AÑADIR LOS ELEMENTOS
		//_buttonScroll->addElementToScroll(_fighterTransform);
		//
		//ClickComponent* clkb = entityManager->addComponent<ClickComponent>(_button);
		//clkb->connect(ClickComponent::JUST_CLICKED, [_buttonScroll]() { /*_buttonScroll->setScrolling(true);*/
		//	if (!_buttonScroll->isScrolling()) _buttonScroll->Scroll(); /*entityManager->getComponent<ScrollComponent>(_button)->Scroll(); */});
		//
		//TriggerComponent* trgb = entityManager->addComponent<TriggerComponent>(_button);
		//trgb->connect(TriggerComponent::CURSOR_ENTERED, []() { std::cout << "ENTERED IN MOVING\n";  });
		//trgb->connect(TriggerComponent::CURSOR_LEFT, []() { std::cout << "LEFT IN MOVING\n";  });
		//
		////NEEDS GETTER OF SCROLLCOMPONENT TO ADD ELEMENTS IF IT HAS
		//
		////NEEDS GETTER OF CLICKCOMPONENT, TRIGGERCOMPONENT, DRAGGINGCOMPONENT FOR EVENTS

		//CREACION DE BOTON
		auto prueba = 
	}
}

void Room1::refresh()
{
}

void Room1::unload()
{
	entityManager->~EntityManager();
}
