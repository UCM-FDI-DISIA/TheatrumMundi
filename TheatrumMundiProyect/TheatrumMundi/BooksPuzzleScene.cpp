#include "BooksPuzzleScene.h"

#include "../src/utils/Vector2D.h";

#include "EntityFactory.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

BooksPuzzleScene::BooksPuzzleScene()
{
}

BooksPuzzleScene::~BooksPuzzleScene()
{
}

void BooksPuzzleScene::init()
{
	if (!isStarted) 
	{
		auto ButtonBook1 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 1 / 7, sdlutils().height() / 2), Vector2D(0, 0), 100, 100, 0);
		auto ButtonBook2 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 3 / 7, sdlutils().height() / 2), Vector2D(0, 0), 100, 100, 0);
		auto ButtonBook3 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 5 / 7, sdlutils().height() / 2), Vector2D(0, 0), 100, 100, 0);
	
		auto number1 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 9 / 21, sdlutils().height() / 5), Vector2D(0, 0), 100, 100, 0);
		auto number2 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 10 / 21, sdlutils().height() / 5), Vector2D(0, 0), 100, 100, 0);
		auto number3 = entityFactory->CreateImageEntity(entityManager, "prueba", EntityFactory::RECTAREA, Vector2D(sdlutils().width() * 11 / 21, sdlutils().height() / 5), Vector2D(0, 0), 100, 100, 0);
	
	}
}

void BooksPuzzleScene::refresh()
{
}

void BooksPuzzleScene::unload()
{
}

bool BooksPuzzleScene::Check()
{
	return false;
}
