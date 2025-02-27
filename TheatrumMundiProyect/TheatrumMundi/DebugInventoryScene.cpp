#include "DebugInventoryScene.h"

#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "Inventory.h"
#include "Hint.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "../src/game/Game.h"

DebugInventoryScene::DebugInventoryScene()
{

}

DebugInventoryScene::~DebugInventoryScene()
{
	delete inv2;
}

/*void DebugInventoryScene::setActive(bool _act)
{
	act = _act;
}*/

void DebugInventoryScene::init()
{
	inv2 = new Inventory();

	if (!isStarted) 
	{
		//All Screen
		auto _screenDetect = entityManager->addEntity();
		auto _screenDetectTr = entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);

		//Create hint entity
		auto _gloves = entityManager->addEntity();
		entityManager->addComponent<Transform>(_gloves, Vector2D(200, 200), Vector2D(0, 0), 200, 200, 0);
		entityManager->addComponent<Image>(_gloves, &sdlutils().images().at("gloves"));

		entityManager->addComponent<RectArea2D>(_gloves);

		ClickComponent* clkInv = entityManager->addComponent<ClickComponent>(_gloves);
		clkInv->connect(ClickComponent::JUST_CLICKED, [this, _gloves]() {

			Hint* gloves = new Hint("gloves", "A pair of gloves", new Image(sdlutils().images().at("gloves")));
			inv2->addItem(gloves);
			std::cout << "Added to inventory " << entityManager->getComponent<Hint>(_gloves)->getDescription() << std::endl;
		});
	}
}
