#include "CreditsScene.h"
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
#include "../src/Components/ScrollComponent.h"

#include "AudioManager.h"
#include "Log.h"
#include "../src/game/Game.h"
#include <string>
using namespace std;

CreditsScene::CreditsScene()
{
}

CreditsScene::~CreditsScene()
{
}

void CreditsScene::init()
{
	if (!isStarted) {

		std::cout << "DENTRO INIT" << std::endl;
		AudioManager& a = AudioManager::Instance();

		a.setListenerPosition(0, 0, 0);

		Sound music = sdlutils().musics().at("sala1"); //cambiar a la melodia que sea
		a.setVolume(music, 0.4);

		//	a.setVolume(music, 1);
		a.setLooping(music, true);
		a.playSound(music);

		// black Background FondoNegro 
		auto _background = entityFactory->CreateImageEntity(entityManager, "fondoMorado", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);
		_backToMainMenu = false;
	
		cred1 = "Hecho por Electric Bogaloo";

		//text
		auto _text1 = entityManager->addEntity(ecs::grp::DEFAULT);
		Transform* trTextLog1 = entityManager->addComponent<Transform>(_text1, Vector2D(0, 0), Vector2D(0, 0), 800, 600, 0);
		SDL_Color colorText1 = { 255, 255, 255, 255 };
		WriteTextComponent<string>* writeLog1 = entityManager->addComponent <WriteTextComponent<string>>(_text1, sdlutils().fonts().at("BASE"), colorText1, &cred1); //write text component
		//entityManager->setActive(_text1, false);

		//scroll area right
		_scrollAreaRight = entityFactory->CreateInteractableEntityScroll(entityManager, "rope", EntityFactory::RECTAREA,
			Vector2D(1200, 400), Vector2D(0, 0), 324 / 3, 893 / 3, 1, areaLayerManager,
			2, 150, EntityFactory::SCROLLNORMAL, 1,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);
		entityManager->removeComponent<Image>(_scrollAreaRight);

		//scroll area Left
		_scrollAreaLeft = entityFactory->CreateInteractableEntityScroll(entityManager, "rope", EntityFactory::RECTAREA,
			Vector2D(1200, 400), Vector2D(0, 0), 324 / 3, 893 / 3, 1, areaLayerManager,
			2, 150, EntityFactory::SCROLLINVERSE, 1,
			EntityFactory::NODRAG,
			ecs::grp::DEFAULT);

		entityManager->removeComponent<Image>(_scrollAreaLeft);

		//moveable courtain right
		_courtainRight = entityFactory->CreateImageEntity(entityManager, "cortinaD", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);

		//moveable courtain left
		_courtainLeft = entityFactory->CreateImageEntity(entityManager, "cortinaI", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);


		//big courtain
		 _bigCourtain = entityFactory->CreateImageEntity(entityManager, "cortinaFija", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);
		
		 //we wait some seconds and we open the courtains
		 SDL_AddTimer(5000, [](Uint32, void* param) -> Uint32 {
			 auto* self = static_cast<decltype(this)>(param);
			 if (!self) return 0;

			 //std::cout << "ABRO CORTINAS" << std::endl;
			 self->openCourtains();

			
			 return 0;
			 }, this);

		 
		
		 //we wait some seconnds and we close them
		 SDL_AddTimer(12000, [](Uint32, void* param) -> Uint32 {
			 auto* self = static_cast<decltype(this)>(param);
			 if (!self) return 0;

			 
			 self->closeCourtains();


			 return 0;
			 }, this);

		 

		 //we wait some more and we return to main menu
		
		 SDL_AddTimer(5000, [](Uint32, void* param) -> Uint32 {
			 auto* self = static_cast<decltype(this)>(param);
			 if (!self) return 0;


			 //self->Game::Instance()->getSceneManager()->popScene();


			 return 0;
			 }, this);
	} 



}

void CreditsScene::openCourtains()
{
	if (!entityManager->getComponent<ScrollComponent>(_scrollAreaRight)->isScrolling()&& !entityManager->getComponent<ScrollComponent>(_scrollAreaLeft)->isScrolling()) {
		auto Scroll = entityManager->getComponent<ScrollComponent>(_scrollAreaRight);
		Scroll->setEndScrollCallback([]() {});
		Scroll->addElementToScroll(entityManager->getComponent<Transform>(_courtainRight));
		entityManager->getComponent<ScrollComponent>(_scrollAreaRight)->Scroll(ScrollComponent::RIGHT);

		auto Scroll2 = entityManager->getComponent<ScrollComponent>(_scrollAreaLeft);
		Scroll2->setEndScrollCallback([]() {});
		Scroll2->addElementToScroll(entityManager->getComponent<Transform>(_courtainLeft));
		entityManager->getComponent<ScrollComponent>(_scrollAreaLeft)->Scroll(ScrollComponent::LEFT);
	}


}

void CreditsScene::closeCourtains()
{
	//std::cout << "CIERRO CORTINAS" << std::endl;
	if (!entityManager->getComponent<ScrollComponent>(_scrollAreaRight)->isScrolling() && !entityManager->getComponent<ScrollComponent>(_scrollAreaLeft)->isScrolling()) {
		auto Scroll = entityManager->getComponent<ScrollComponent>(_scrollAreaRight);
		Scroll->setEndScrollCallback([]() {});
		Scroll->addElementToScroll(entityManager->getComponent<Transform>(_courtainRight));
		entityManager->getComponent<ScrollComponent>(_scrollAreaRight)->Scroll(ScrollComponent::LEFT);

		auto Scroll2 = entityManager->getComponent<ScrollComponent>(_scrollAreaRight);
		Scroll2->setEndScrollCallback([]() {});
		Scroll2->addElementToScroll(entityManager->getComponent<Transform>(_courtainLeft));
		entityManager->getComponent<ScrollComponent>(_scrollAreaLeft)->Scroll(ScrollComponent::RIGHT);
	}
}
