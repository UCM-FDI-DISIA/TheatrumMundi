#include "CalendearMobileTestScene.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"

CalendearMobileTestScene::CalendearMobileTestScene()
{
}

CalendearMobileTestScene::~CalendearMobileTestScene()
{
}

void CalendearMobileTestScene::init()
{
	if (!isStarted) {

		isStarted = true;
		//Creation of the Calendear Button
		auto _calendearButton = entityFactory->CreateInteractableEntity(entityManager, "calendear", entityFactory->RECTAREA,Vector2D(600, 100), Vector2D(0, 0), 200, 200, 0, areaLayerManager, entityFactory->NODRAG,ecs::grp::DEFAULT);
		ClickComponent* calendearClick = entityManager->addComponent<ClickComponent>(_calendearButton);

		//Creation of the Calendear Zoom
		calendearClick->connect(ClickComponent::JUST_CLICKED, [this, _calendearButton]()
			{
				auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "calendearZoom", Vector2D(10, 10), Vector2D(0, 0), 1300, 900, 0, ecs::grp::DEFAULT);
				//this->startDialogue(Calendario);
				_calendearButton->getMngr()->setActive(_calendearButton, false);
				auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "exit", entityFactory->RECTAREA, Vector2D(10, 10), Vector2D(0, 0), 50, 50, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::DEFAULT);
				ClickComponent* click2 = entityManager->addComponent<ClickComponent>(_quitButton);

				//Exit Calendear Zoom
				click2->connect(ClickComponent::JUST_CLICKED, [this, _calendearZoom, _calendearButton,_quitButton]()
					{
						_calendearZoom->getMngr()->setActive(_calendearZoom, false);
						_calendearButton->getMngr()->setActive(_calendearButton, true);
						_quitButton->getMngr()->setActive(_quitButton, false);
					});
			});

		//Creation of the mobile Button 
		auto _mobileButton = entityFactory->CreateInteractableEntity(entityManager, "mobile", entityFactory->RECTAREA, Vector2D(900, 300), Vector2D(0, 0), 100, 100, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::DEFAULT);
		ClickComponent* mobileclick = entityManager->addComponent<ClickComponent>(_mobileButton);

		//Activation of Mobile Zoom
		mobileclick->connect(ClickComponent::JUST_CLICKED, [this, _mobileButton]()
			{
				auto _mobileZoom = entityFactory->CreateImageEntity(entityManager, "mobileZoom", Vector2D(10, 10), Vector2D(0, 0), 1300, 900, 0, ecs::grp::DEFAULT);
				//this->startDialogue(Movil);
				_mobileButton->getMngr()->setActive(_mobileButton, false);
				auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "exit", entityFactory->RECTAREA, Vector2D(10, 10), Vector2D(0, 0), 50, 50, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::DEFAULT);
				ClickComponent* click2 = entityManager->addComponent<ClickComponent>(_quitButton);

				//Exit mobile Zoom
				click2->connect(ClickComponent::JUST_CLICKED, [this, _mobileZoom, _mobileButton, _quitButton]()
					{
						_mobileZoom->getMngr()->setActive(_mobileZoom, false);
						_mobileButton->getMngr()->setActive(_mobileButton, true);
						_quitButton->getMngr()->setActive(_quitButton, false);
					});
			});


	}

}


void CalendearMobileTestScene::unload()
{
}
