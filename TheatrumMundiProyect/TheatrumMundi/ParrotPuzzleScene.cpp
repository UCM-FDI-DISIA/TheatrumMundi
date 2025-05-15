#include "ParrotPuzzleScene.h"

ParrotPuzzleScene::ParrotPuzzleScene()
{

}

ParrotPuzzleScene::~ParrotPuzzleScene()
{

}

void ParrotPuzzleScene::init(SceneRoomTemplate* sr)
{

}

void ParrotPuzzleScene::unload()
{

}

bool ParrotPuzzleScene::Check()
{
	return false;
}

// Init Setting Methods

void ParrotPuzzleScene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void ParrotPuzzleScene::_setRoomBackground()
{
	teaCupBackground = entityFactory->CreateImageEntity(entityManager, "Parrot", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);
}