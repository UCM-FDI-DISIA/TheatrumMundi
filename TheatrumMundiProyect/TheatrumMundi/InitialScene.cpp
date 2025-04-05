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

		a.setListenerPosition(0, 0, 0);

		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);

		Sound music = sdlutils().musics().at("sala1");
		a.setVolume(music, 0.4);
		
	//	a.setVolume(music, 1);
		a.setLooping(music, true);
		a.playSound(music);

		//clear the log
		Game::Instance()->getLog()->cleanLogList();
		Game::Instance()->getLog()->cleanRenderedList();

		//Background
		//ENTIDADCONENTITYFACTORY
		auto _background = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);

		
		//Title
		//ENTIDADCONENTITYFACTORY
		auto _title = entityFactory->CreateImageEntity(entityManager, "Title", Vector2D(300, 126), Vector2D(0, 0), 735, 135, 0, ecs::grp::DEFAULT);
		

		//Tutorial button
		//ENTIDADCONENTITYFACTORY
		auto _tutobtn = entityFactory->CreateInteractableEntity(entityManager, "TutorialButton", EntityFactory::RECTAREA, Vector2D(482, 302), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		ClickComponent* clktuto = entityManager->getComponent<ClickComponent>(_tutobtn);

		clktuto->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			_loadimg->getMngr()->setActive(_loadimg, true);

			Game::Instance()->render();
			Game::Instance()->getSceneManager()->loadScene(TUTORIAL_SCENE);


			});


		//Start button room1
		//ENTIDADCONENTITYFACTORY
		if (tutorialCompleted) {
			auto _startbtn = entityFactory->CreateInteractableEntity(entityManager, "NewGame", EntityFactory::RECTAREA, Vector2D(482, 426), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			ClickComponent* clk = entityManager->getComponent<ClickComponent>(_startbtn);

			clk->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

				AudioManager::Instance().playSound(buttonSound);
				_loadimg->getMngr()->setActive(_loadimg, true);

				Game::Instance()->render();
				Game::Instance()->getSceneManager()->loadScene(MIDDLE_ROOM);


				});
		}

		else {
			auto _startbtn = entityFactory->CreateInteractableEntity(entityManager, "NewGameLocked", EntityFactory::RECTAREA, Vector2D(482, 426), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

			ClickComponent* clk = entityManager->getComponent<ClickComponent>(_startbtn);

			clk->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

				AudioManager::Instance().playSound(buttonSound);

				});
		}
		//Exit 
		//ENTIDADCONENTITYFACTORY
		auto _exitbtn = entityFactory->CreateInteractableEntity(entityManager, "Exit", EntityFactory::RECTAREA, Vector2D(482, 550), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);

		ClickComponent* clkext = entityManager->getComponent<ClickComponent>(_exitbtn);
		clkext->connect(ClickComponent::JUST_CLICKED, [buttonSound]() 
			{
				AudioManager::Instance().playSound(buttonSound);
				Game::Instance()->exit();
				});

		//ENTIDADCONENTITYFACTORY
		_loadimg = entityFactory->CreateImageEntity(entityManager, "loading", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);
	
	}
	_loadimg->getMngr()->setActive(_loadimg, false);
}

