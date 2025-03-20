#include "DialogTestScene.h"
#include "../src/Components/WriteTextComponent.h"
#include "../src/Components/Transform.h"
#include "../src/game/Game.h"
#include "DialogueManager.h"
DialogTestScene::DialogTestScene(): SceneRoomTemplate()
{
}

DialogTestScene::~DialogTestScene()
{
}

void DialogTestScene::init()
{
	if (!isStarted) {
		/*
		auto _textTest = entityManager->addEntity();
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		entityManager->addComponent<WriteTextComponent>(_textTest);
		*/
		dialogueManager->ReadDialogue("SalaIntermedia1");
	}

}

void DialogTestScene::refresh()
{
}

void DialogTestScene::unload()
{
}
