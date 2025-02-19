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
		entityManager->addComponent<Transform>(_startbtn, Vector2D(200, 200), Vector2D(0, 0), 200, 200, 0);
		entityManager->addComponent<Image>(_startbtn, &sdlutils().images().at("init"));

		entityManager->addComponent<RectArea2D>(_startbtn);

		ClickComponent* clk = entityManager->addComponent<ClickComponent>(_startbtn);
		clk->connect(ClickComponent::JUST_CLICKED, []() {Game::Instance()->getSceneManager()->loadScene(2);});
		//Exit 
		auto _exitbtn = entityManager->addEntity();
		entityManager->addComponent<Transform>(_exitbtn, Vector2D(300, 400), Vector2D(0, 0), 200, 200, 0);
		entityManager->addComponent<Image>(_exitbtn, &sdlutils().images().at("exit"));

		entityManager->addComponent<RectArea2D>(_exitbtn);

		ClickComponent* clkext = entityManager->addComponent<ClickComponent>(_exitbtn);
		clkext->connect(ClickComponent::JUST_CLICKED, []() {Game::Instance()->exit();});


	}
}

void InitialScene::refresh()
{
}
