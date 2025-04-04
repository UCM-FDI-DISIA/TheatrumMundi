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
	//delete inv2;
}

/*void DebugInventoryScene::setActive(bool _act)
{
	act = _act;
}*/

void DebugInventoryScene::init()
{
	//inv2 = new Inventory();
	//std::vector<Vector2D> itemPositions = { {100, 100}, {100, 250}, {100, 300}, {100, 400}, {100, 500} };

	if (!isStarted) 
	{
		//All Screen
		//ENTIDADCONENTITYFACTORY
		//auto _screenDetect = entityManager->addEntity();
		//auto _screenDetectTr = entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);

		//Create hints entitys
		//ENTIDADCONENTITYFACTORY
		//auto _gloves = entityManager->addEntity();
		//entityManager->addComponent<Transform>(_gloves, Vector2D(500, 500), Vector2D(0, 0), 100, 100, 0);
		//entityManager->addComponent<Image>(_gloves, &sdlutils().images().at("gloves"));
		//
		//entityManager->addComponent<RectArea2D>(_gloves);
		//
		//ClickComponent* clkInv = entityManager->addComponent<ClickComponent>(_gloves);
		//clkInv->connect(ClickComponent::JUST_CLICKED, [this, _gloves, itemPositions]() {
		//
		//	_gloves->getMngr()->setActive(_gloves, false);  // Desactivate the gloves entity
		//
		//	// Create a hint and add it to the inventory
		//	Hint* glovesHint = new Hint("gloves", "A pair of gloves", &sdlutils().images().at("gloves"));
		//	inv2->addItem(glovesHint);
		//
		//	Vector2D itemPos = itemPositions[inv2->hints.size() % itemPositions.size()]; // Calculate the position of the hint in the inventory
		//
		//	inv2->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, glovesHint->getID(), EntityFactory::RECTAREA, itemPos, Vector2D(0, 0), 150, 150, 0, areaLayerManager, EntityFactory::DRAG, grp::DEFAULT));
		//	inv2->hints.back()->getMngr()->setActive(inv2->hints.back(),false);
		//	glovesHint->setActive(false);  // Desactivated inicially for the inventory
		//	//std::cout << "Added to inventory. Active state: " << gloves->getActive() << std::endl;
		//});

		//ENTIDADCONENTITYFACTORY
		//auto _spoon = entityManager->addEntity();
		//entityManager->addComponent<Transform>(_spoon, Vector2D(500, 600), Vector2D(0, 0), 100, 100, 0);
		//entityManager->addComponent<Image>(_spoon, &sdlutils().images().at("kei"));
		//
		//entityManager->addComponent<RectArea2D>(_spoon);
		//
		//ClickComponent* clkInv2 = entityManager->addComponent<ClickComponent>(_spoon);
		//clkInv2->connect(ClickComponent::JUST_CLICKED, [this, _spoon, itemPositions]() {
		//
		//	_spoon->getMngr()->setActive(_spoon, false);  // Desactivate the gloves entity
		//
		//	// Create a hint and add it to the inventory
		//	Hint* spoonHint = new Hint("kei", "Una cuchara de metal", &sdlutils().images().at("kei"));
		//	inv2->addItem(spoonHint);
		//
		//	Vector2D itemPos = itemPositions[inv2->hints.size() % itemPositions.size()];
		//
		//	inv2->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, spoonHint->getID(), EntityFactory::RECTAREA, itemPos, Vector2D(0, 0), 150, 150, 0, areaLayerManager, EntityFactory::DRAG, grp::DEFAULT));
		//	inv2->hints.back()->getMngr()->setActive(inv2->hints.back(), false);
		//	spoonHint->setActive(false);  // Desactivated inicially for the inventory
		//	//std::cout << "Added to inventory. Active state: " << gloves->getActive() << std::endl;
		//});



		//inv button
		//ENTIDADCONENTITYFACTORY
		//auto _button = entityManager->addEntity();
		//entityManager->addComponent<Transform>(_button, Vector2D(800, 200), Vector2D(0, 0), 200, 200, 0);
		//entityManager->addComponent<Image>(_button, &sdlutils().images().at("exit"));
		//
		//entityManager->addComponent<RectArea2D>(_button);
		//
		//
		//ClickComponent* toggleInventoryClick = entityManager->addComponent<ClickComponent>(_button);
		//toggleInventoryClick->connect(ClickComponent::JUST_CLICKED, [this]() {
		//
		//	inv2->setActive(!inv2->getActive());  // Toggle the inventory
		//
		//	// If the inventory is active, activate the items
		//	if (inv2->getActive()) {
		//
		//		for (int i = 0; i < inv2->getItemNumber(); ++i) {
		//			inv2->hints[i]->getMngr()->setActive(inv2->hints[i], true);  // Activate the hints
		//		}
		//	}
		//	else {
		//
		//		for (int i = 0; i < inv2->getItemNumber(); ++i) {
		//			inv2->hints[i]->getMngr()->setActive(inv2->hints[i], false);  // Activate the hints
		//		}
		//	}
		//});

	}
}
