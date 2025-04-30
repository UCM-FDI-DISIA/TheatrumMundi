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


#include "AudioManager.h"
#include "Log.h"
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

		AudioManager& a = AudioManager::Instance();

		a.setListenerPosition(0, 0, 0);

		Sound music = sdlutils().musics().at("sala1"); //cambiar a la melodia que sea
		a.setVolume(music, 0.4);

		//	a.setVolume(music, 1);
		a.setLooping(music, true);
		a.playSound(music);

		// black Background
		auto _background = entityFactory->CreateImageEntity(entityManager, "FondoNegro", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DEFAULT);

		// AnimationFrames
		courtainAnimationFrames.clear();
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom2"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom3"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom4"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomKei"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomSol"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoomLucy"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("TutorialZoom5"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("FondoNegro"));
		courtainAnimationFrames.push_back(&sdlutils().images().at("FondoNegro"));


		currentFrameIndex = 0;

		//frameTimer.resetTime();
		//startDialogue("Tutorial8");
		isAnimating = true;
		// Change to first texture
		courtainsBackground->getMngr()->getComponent<Image>(courtainsBackground)->setTexture(courtainAnimationFrames[0]);

		closedSecondTime = false;

		string creds = "a";

		//texto
		auto _text = entityManager->addEntity(ecs::grp::DEFAULT);
		Transform* trTextLog = entityManager->addComponent<Transform>(_text, Vector2D(0, 0), Vector2D(0, 0), 800, 600, 0);
		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<string>* writeLog =
			entityManager->addComponent <WriteTextComponent<string>>(_text, sdlutils().fonts().at("BASE"), colorText, &creds); //write text component
		entityManager->setActive(_text, false);
		
	}





}

void CreditsScene::openCourtains()
{
	//OPENS COURTAINS SHOW CREDITS
	//ANIMATION
	/*if (isAnimating && frameTimer.currRealTime() > 400 && (currentFrameIndex == 4 || currentFrameIndex == 6 || currentFrameIndex == 8)) // 0.4 sec
	{
		currentFrameIndex++;


		if (currentFrameIndex >= courtainAnimationFrames.size()) {

			isAnimating = false;
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			Game::Instance()->reset();
			return;
		}


		courtainsBackground->getMngr()->getComponent<Image>(courtainsBackground)->setTexture(courtainAnimationFrames[currentFrameIndex]);


		//frameTimer.resetTime();
	}
	else if (isAnimating && frameTimer.currRealTime() > 1000) {

		currentFrameIndex++;


		if (currentFrameIndex >= courtainAnimationFrames.size()) {

			isAnimating = false;
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			Game::Instance()->reset();
			return;
		}


		courtainsBackground->getMngr()->getComponent<Image>(courtainsBackground)->setTexture(courtainAnimationFrames[currentFrameIndex]);


		//frameTimer.resetTime();

	}*/
}

void CreditsScene::closeCourtains()
{

	//GOES BACK TO MAIN MENU if the secnd time
}
