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
InitialScene::InitialScene()
{
}

InitialScene::~InitialScene()
{

}

void InitialScene::init()
{
	if (!isStarted) {
		//Start room1
		
		auto _startbtn = entityManager->addEntity();
		entityManager->addComponent<Transform>(_startbtn, Vector2D(0, 0), Vector2D(0, 0), 674*2, 374*2, 0);
		entityManager->addComponent<Image>(_startbtn, &sdlutils().images().at("kei"));

		entityManager->addComponent<RectArea2D>(_startbtn);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_startbtn);
		clk->connect(ClickComponent::JUST_CLICKED, [this]() {
		_loadimg->getMngr()->setActive(_loadimg, true);
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->loadScene(1);});
		//Exit 
		auto _exitbtn = entityManager->addEntity();
		entityManager->addComponent<Transform>(_exitbtn, Vector2D(300, 400), Vector2D(0, 0), 200, 200, 0);
	//	entityManager->addComponent<Image>(_exitbtn, &sdlutils().images().at("exit"));

		entityManager->addComponent<RectArea2D>(_exitbtn);

		ClickComponent* clkext = entityManager->addComponent<ClickComponent>(_exitbtn);
		clkext->connect(ClickComponent::JUST_CLICKED, []() {Game::Instance()->exit();});

		_loadimg = entityManager->addEntity();
		entityManager->addComponent<Transform>(_loadimg, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->addComponent<Image>(_loadimg, &sdlutils().images().at("loading"));
	
	}
	_loadimg->getMngr()->setActive(_loadimg, false);
}

void InitialScene::refresh()
{
}
