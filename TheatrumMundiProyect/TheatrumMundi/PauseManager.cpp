#include "PauseManager.h"
#include "EntityFactory.h"
#include "Area2DLayerManager.h"
#include"../src/sdlutils/SDLUtils.h"
#include "SceneTemplate.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "MouseIntaractionComponent.h"
#include "../src/ecs/Manager.h"
#include "AnimPauseComponent.h"
#include "SceneManager.h"
#include "Area2D.h"
#include "RectArea2D.h"

PauseManager::PauseManager() : _pauseActive(false), _scene(nullptr), _backgroundNotInteractable(nullptr), _reanudePauseButton(nullptr), _exitPauseButton(nullptr)
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
	std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
	audioManager().setVolume(buttonSound, 0.2);
	
	auto _backgroundPause = entityFactory->CreateImageEntity(entityManager, "MenuBackground", Vector2D(443, 250), Vector2D(0, 0), 450, 245, 0, ecs::grp::BACKGROUNDPAUSE);
	entityManager->addComponent<AnimPauseComponent>(_backgroundPause);
	animElems.push_back(_backgroundPause);
	entityManager->setActive(_backgroundPause, false);

	//ENTIDADCONENTITYFACTORY
	_backgroundNotInteractable = entityFactory->CreateInteractableEntity(entityManager,"EmptyImage",EntityFactory::RECTAREA,Vector2D(0,0),Vector2D(0,0),1349,748,0,areaLayerManager,EntityFactory::NODRAG, ecs::grp::BACKGROUNDPAUSE);
	entityManager->addComponent<AnimPauseComponent>(_backgroundNotInteractable);
	animElems.push_back(_backgroundNotInteractable);
	entityManager->setActive(_backgroundNotInteractable, false);
	
	//GO TO ROOM
	_reanudePauseButton = entityFactory->CreateInteractableEntity(entityManager, "Continue", EntityFactory::RECTAREA, Vector2D(482, 280), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTPAUSE);
	entityManager->addComponent<AnimPauseComponent>(_reanudePauseButton);
	animElems.push_back(_reanudePauseButton);
	entityManager->setActive(_reanudePauseButton, false);
	ClickComponent* buttonReanudePauseClick = entityManager->getComponent<ClickComponent>(_reanudePauseButton);
	buttonReanudePauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		audioManager().playSound(buttonSound);
		for (auto a : animElems) entityManager->getComponent<AnimPauseComponent>(a)->endPauseAnim();
		});

	//GO TO THE INITIALMENU SCENE
	_exitPauseButton = entityFactory->CreateInteractableEntity(entityManager, "ExitToMenu", EntityFactory::RECTAREA, Vector2D(482, 398), Vector2D(0, 0), 367, 67, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTPAUSE);
	entityManager->addComponent<AnimPauseComponent>(_exitPauseButton);
	animElems.push_back(_exitPauseButton);
	entityManager->setActive(_exitPauseButton, false);
	ClickComponent* buttonExitPauseClick = entityManager->getComponent<ClickComponent>(_exitPauseButton);
	buttonExitPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		audioManager().playSound(buttonSound);
		for (auto a : animElems) entityManager->getComponent<AnimPauseComponent>(a)->endPauseAnim();

		Game::Instance()->setReset();

		});

	//GO TO THE PAUSE SCENE
	_openPauseButton = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	ClickComponent* buttonOpenPauseClick = entityManager->getComponent<ClickComponent>(_openPauseButton);
	buttonOpenPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, entityManager]() {
		audioManager().playSound(buttonSound);
		entityManager->setActiveGroup(ecs::grp::INTERACTPAUSE, true);
		entityManager->setActiveGroup(ecs::grp::BACKGROUNDPAUSE, true);
		entityManager->setActive(_backgroundNotInteractable,true);
		for (auto a : animElems) entityManager->getComponent<AnimPauseComponent>(a)->startPauseAnim();
		});
}

