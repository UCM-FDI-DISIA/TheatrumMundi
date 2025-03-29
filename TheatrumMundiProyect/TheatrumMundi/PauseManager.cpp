#include "PauseManager.h"
#include "EntityFactory.h"
#include "Area2DLayerManager.h"
#include "../src/ecs/Manager.h"
#include"../src/sdlutils/SDLUtils.h"
#include "SceneTemplate.h"

void PauseManager::SetPauseActive(bool active)
{
}

bool PauseManager::GetPauseActive()
{
	return false;
}

void PauseManager::InitPause(EntityFactory* entityFactory, EntityManager* entityManager, Area2DLayerManager* areaLayerManager)
{
	auto _backgroundPause = entityFactory->CreateImageEntity(entityManager,"fondoPruebaLog",Vector2D(0,0),Vector2D(0,0),100,100,0,ecs::grp::BACKGROUND);
}

