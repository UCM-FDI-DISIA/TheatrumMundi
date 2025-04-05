#include "InitialScene.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "../../TheatrumMundiProyect/src/game/Game.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"

#include "AudioManager.h"

#include "GameSave.h"
#include "Log.h"

InitialScene::InitialScene()
{
}

InitialScene::~InitialScene()
{

}

void InitialScene::init()
{
	if (!isStarted) {

		GameSave save("savegame.dat");
		bool tutorialCompleted = save.isTutoCompleted();


		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);


		//Background
		//ENTIDADCONENTITYFACTORY
		auto _background = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);

		
		//Title
		//ENTIDADCONENTITYFACTORY
		auto _title = entityFactory->CreateImageEntity(entityManager, "Title", Vector2D(300, 187), Vector2D(0, 0), 735, 135, 0, ecs::grp::DEFAULT);
		
		//Start button room1
		//ENTIDADCONENTITYFACTORY
		auto _startbtn = entityFactory->CreateInteractableEntity(entityManager, "NewGame", EntityFactory::RECTAREA, Vector2D(482, 374), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		ClickComponent* clk = entityManager->getComponent<ClickComponent>(_startbtn);
		clk->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

		AudioManager::Instance().playSound(buttonSound);
		_loadimg->getMngr()->setActive(_loadimg, true);
		
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->loadScene(MIDDLE_ROOM);
			
			
		});
		
		//Exit 
		//ENTIDADCONENTITYFACTORY
		auto _exitbtn = entityFactory->CreateInteractableEntity(entityManager, "Exit", EntityFactory::RECTAREA, Vector2D(482, 498), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		ClickComponent* clkext = entityManager->getComponent<ClickComponent>(_exitbtn);
		clkext->connect(ClickComponent::JUST_CLICKED, [buttonSound]() 
			{
				AudioManager::Instance().playSound(buttonSound);
				_loadimg->getMngr()->setActive(_loadimg, true);
				Game::Instance()->render();
				Game::Instance()->getSceneManager()->loadScene(MIDDLE_ROOM);
				});
		}
		else {
			// Opcional: A�adir un sonido o feedback cuando intentan clickear sin haber completado el tutorial
			clk->connect(ClickComponent::JUST_CLICKED, [buttonSound]() {
				AudioManager::Instance().playSound(buttonSound);
				// Puedes mostrar un mensaje o tooltip aqu�
				});;
		}
			//Exit 
			auto _exitbtn = entityManager->addEntity();
			entityManager->addComponent<Transform>(_exitbtn, Vector2D(1349 / 2.8, 748 / 1.2), Vector2D(0, 0), 1470 / 4, 270 / 4, 0);
			entityManager->addComponent<Image>(_exitbtn, &sdlutils().images().at("Exit"));

		//ENTIDADCONENTITYFACTORY
		_loadimg = entityFactory->CreateImageEntity(entityManager, "loading", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);
	
	}
	_loadimg->getMngr()->setActive(_loadimg, false);
}

