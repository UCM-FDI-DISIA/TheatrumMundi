#include "DebugInventoryScene.h"

#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

#include "Inventory.h"
#include "Hint.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/DragComponent.h"
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

		//Create hints entitys
		auto _gloves = entityManager->addEntity();
		entityManager->addComponent<Transform>(_gloves, Vector2D(500, 500), Vector2D(0, 0), 100, 100, 0);
		entityManager->addComponent<Image>(_gloves, &sdlutils().images().at("gloves"));

		entityManager->addComponent<RectArea2D>(_gloves);

		ClickComponent* clkInv = entityManager->addComponent<ClickComponent>(_gloves);
		clkInv->connect(ClickComponent::JUST_CLICKED, [this, _gloves]() {

			_gloves->getMngr()->setActive(_gloves, false);  // Desactivate the gloves entity

			// Create a hint and add it to the inventory
			Hint* glovesHint = new Hint("gloves", "A pair of gloves", &sdlutils().images().at("gloves"));
			inv2->addItem(glovesHint);
			glovesHint->setActive(false);  // Desactivated inicially for the inventory
			//std::cout << "Added to inventory. Active state: " << gloves->getActive() << std::endl;
		});

		auto _spoon = entityManager->addEntity();
		entityManager->addComponent<Transform>(_spoon, Vector2D(500, 600), Vector2D(0, 0), 100, 100, 0);
		entityManager->addComponent<Image>(_spoon, &sdlutils().images().at("spoon"));

		entityManager->addComponent<RectArea2D>(_spoon);

		ClickComponent* clkInv2 = entityManager->addComponent<ClickComponent>(_spoon);
		clkInv2->connect(ClickComponent::JUST_CLICKED, [this, _spoon]() {

			_spoon->getMngr()->setActive(_spoon, false);  // Desactivate the gloves entity

			// Create a hint and add it to the inventory
			Hint* spoonHint = new Hint("spoon", "Una cuchara de metal", &sdlutils().images().at("spoon"));
			inv2->addItem(spoonHint);
			spoonHint->setActive(false);  // Desactivated inicially for the inventory
			//std::cout << "Added to inventory. Active state: " << gloves->getActive() << std::endl;
		});



		//inv button
		auto _button = entityManager->addEntity();
		entityManager->addComponent<Transform>(_button, Vector2D(800, 200), Vector2D(0, 0), 200, 200, 0);
		entityManager->addComponent<Image>(_button, &sdlutils().images().at("exit"));

		entityManager->addComponent<RectArea2D>(_button);

		ClickComponent* toggleInventoryClick = entityManager->addComponent<ClickComponent>(_button);
		toggleInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, _gloves]() {

			inv2->setActive(!inv2->getActive());  // Toggle the inventory

			// If the inventory is active, activate the items
			if (inv2->getActive()) {
				for (auto& item : inv2->getItems()) {
					item->setActive(true);
					//_gloves->getMngr()->setActive(_gloves, true);  // Activate the gloves entity
				}
			}
			else {
				for (auto& item : inv2->getItems()) {
					item->setActive(false);
					//_gloves->getMngr()->setActive(_gloves, false);  // Desactivate the gloves entity
				}
			}

			inv2->render();

			std::cout << "Inventory active state: " << inv2->getActive() << std::endl;
			/*inv2->setActive(!inv2->getActive());
			inv2->render();
			auto glovesHint = inv2->getItem("gloves"); // Obtain the hint from the inventory
			if (glovesHint) {
				glovesHint->setActive(!glovesHint->getActive()); // Activate or deactivate the hint
			}
			std::cout << "Inventory active state: " << inv2->getActive() << std::endl;*/
		});

	}
}
