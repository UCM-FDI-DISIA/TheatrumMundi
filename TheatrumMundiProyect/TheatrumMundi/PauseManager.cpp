#include "PauseManager.h"
#include "EntityFactory.h"
#include "Area2DLayerManager.h"
#include"../src/sdlutils/SDLUtils.h"
#include "SceneTemplate.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "MouseIntaractionComponent.h"
#include "../src/ecs/Manager.h"

#include "SceneManager.h"

PauseManager::PauseManager() : _pauseActive(false), _scene(nullptr)
{
}

PauseManager::~PauseManager()
{
}

void PauseManager::SetPauseActive(bool active)
{
	_pauseActive = active;
}

void PauseManager::setScene(SceneTemplate* scene)
{
	_scene = scene;
}

bool PauseManager::GetPauseActive()
{
	return _pauseActive;
}

void PauseManager::Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager)
{
	Sound buttonSound = sdlutils().soundEffects().at("boton");
	AudioManager::Instance().setVolume(buttonSound, 0.2);

	auto _backgroundPause = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(450, 250), Vector2D(0, 0), 450, 250, 0, ecs::grp::BACKGROUNDPAUSE);
	entityManager->setActive(_backgroundPause, false);

	//GO TO ROOM
	auto reanudePauseButton = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(470, 270), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTPAUSE);
	entityManager->setActive(reanudePauseButton, false);
	ClickComponent* buttonReanudePauseClick = entityManager->getComponent<ClickComponent>(reanudePauseButton);
	buttonReanudePauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		AudioManager::Instance().playSound(buttonSound);
		entityManager->setActiveGroup(ecs::grp::INTERACTPAUSE, false);
		entityManager->setActiveGroup(ecs::grp::BACKGROUNDPAUSE, false);
		});

	//GO TO THE INITIALMENU SCENE
	auto exitPauseButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(470, 370), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTPAUSE);
	entityManager->setActive(exitPauseButton, false);
	ClickComponent* buttonExitPauseClick = entityManager->getComponent<ClickComponent>(exitPauseButton);
	buttonExitPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		AudioManager::Instance().playSound(buttonSound);
		entityManager->setActiveGroup(ecs::grp::INTERACTPAUSE, false);
		entityManager->setActiveGroup(ecs::grp::BACKGROUNDPAUSE, false);
		Game::Instance()->getSceneManager()->loadScene(SceneName::INITIAL_MENU);
		});

	//GO TO THE PAUSE SCENE
	auto openPauseButton = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	ClickComponent* buttonOpenPauseClick = entityManager->getComponent<ClickComponent>(openPauseButton);
	buttonOpenPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		AudioManager::Instance().playSound(buttonSound);
		entityManager->setActiveGroup(ecs::grp::INTERACTPAUSE, true);
		entityManager->setActiveGroup(ecs::grp::BACKGROUNDPAUSE, true);
		});
}

