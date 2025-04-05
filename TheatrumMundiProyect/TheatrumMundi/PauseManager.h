#pragma once
#include "ecs.h"

class EntityFactory;
class SceneTemplate;
class ClickComponent;
class Area2DLayerManager;

class PauseManager {
private:
	bool _pauseActive;
	SceneTemplate* _scene;
public:
	PauseManager();
	~PauseManager();
	void SetPauseActive(bool active);
	void setScene(SceneTemplate* scene);
	bool GetPauseActive();

	ecs::entity_t _backgroundNotInteractable;
	ecs::entity_t _getbackgroundNotInteractable(){ return _backgroundNotInteractable; }
	ecs::entity_t _reanudePauseButton;
	ecs::entity_t _getreanudePauseButton() { return _reanudePauseButton; }
	ecs::entity_t _exitPauseButton;
	ecs::entity_t _getexitPauseButton() { return _exitPauseButton; }
	ecs::entity_t _openPauseButton;
	ecs::entity_t _getopenPauseButton() { return _openPauseButton; }

	void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager);
};