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

InitialScene::InitialScene()
{
}

InitialScene::~InitialScene()
{
	
}

void InitialScene::init()
{
	if (!isStarted) {

		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);


		//Background
		auto _background = entityManager->addEntity();
		entityManager->addComponent<Transform>(_background, Vector2D(0, 0), Vector2D(0, 0),2019/1.5, 1122/1.5, 0);
		entityManager->addComponent<Image>(_background, &sdlutils().images().at("Room"));
		
		
		//Title
		auto _title = entityManager->addEntity();
		entityManager->addComponent<Transform>(_title, Vector2D(1349 / 4.5, 748 /4), Vector2D(0, 0), 1470 / 2, 270 / 2, 0);
		entityManager->addComponent<Image>(_title, &sdlutils().images().at("Title"));
		
		//Start button room1
		auto _startbtn = entityManager->addEntity();
		entityManager->addComponent<Transform>(_startbtn, Vector2D(1348/2.8, 748/2), Vector2D(0, 0), 1470/4, 270/4, 0);
		entityManager->addComponent<Image>(_startbtn, &sdlutils().images().at("NewGame"));

		entityManager->addComponent<RectArea2D>(_startbtn);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_startbtn);
		clk->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

		AudioManager::Instance().playSound(buttonSound);
		_loadimg->getMngr()->setActive(_loadimg, true);
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->loadScene(1);});
		
		//Exit 
		auto _exitbtn = entityManager->addEntity();
		entityManager->addComponent<Transform>(_exitbtn, Vector2D(1349 / 2.8, 748 / 1.5), Vector2D(0, 0), 1470 / 4, 270 / 4, 0);
		entityManager->addComponent<Image>(_exitbtn, &sdlutils().images().at("Exit"));

		entityManager->addComponent<RectArea2D>(_exitbtn);

		ClickComponent* clkext = entityManager->addComponent<ClickComponent>(_exitbtn);
		clkext->connect(ClickComponent::JUST_CLICKED, [buttonSound]() 
			{
				AudioManager::Instance().playSound(buttonSound);
				Game::Instance()->exit();
			});

		_loadimg = entityManager->addEntity();
		entityManager->addComponent<Transform>(_loadimg, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->addComponent<Image>(_loadimg, &sdlutils().images().at("loading"));
	
	}
	_loadimg->getMngr()->setActive(_loadimg, false);
}

void InitialScene::refresh()
{
}
